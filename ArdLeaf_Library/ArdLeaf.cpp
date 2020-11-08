#include "Arduino.h"
#include "ArdLeaf.h"
#include "mcp_can.h"
#include <SPI.h>

ArdLeaf::ArdLeaf(int pin_cs, int pin_int) {
  canEV = new MCP_CAN(pin_cs);
  pinMode(pin_int, INPUT);

  pinCS = pin_cs;
  pinINT = pin_int;
}

void ArdLeaf::connect() {
  delay(1000);
  Serial.print("Connecting to CAN...  ");
  
  if(canEV->begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("Connected!");
  } else {
    Serial.println("Error!");
  }

  canEV->setMode(MCP_NORMAL);

  delay(3000);
}

void ArdLeaf::update() {
  if( !digitalRead(pinINT) ) { // Check if data is available
    canEV->readMsgBuf(&msgId, &msgLen, msg);
    if (msgId == 0x5bc) { // SOC (With degradation)
      soc_gids = (msg[0] << 2) | (msg[1] >> 6);
      soc_percent = (soc_gids / MAX_GIDS) * 100.0F;
      soh = getValue(msg[4], 1, 7);

    } else if (msgId == 0x55b) { // SOC (Without degradation)
      soc = ( (msg[0] << 2) | (msg[1] >> 6) ) / 10.0F;

    } else if (msgId == 0x1db) { // Voltage and current
      battery_voltage = ( (msg[2] << 2) | (msg[3] >> 6) ) / 2.0F;

      // sent by the LBC, measured inside the battery box
      // current is 11 bit twos complement big endian starting at bit 0
      int16_t current = ((int16_t) msg[0] << 3) | (msg[1] & 0xe0) >> 5;
      if (current & 0x0400) { // negative so extend the sign bit
        current |= 0xf800;
      }
      battery_current = -current / 2.0f;

      battery_kw = (battery_current * battery_voltage)/1000.0F;
      
    } else if (msgId == 0x284) { // Speed sensors
      speed = ( (msg[4] << 8) | msg[5] ) / 92;

    } else if (msgId == 0x54b) { // A/C
      ac_fan_speed = (msg[4] / 8);

    } else if (msgId == 0x11a) { // Shift controller (Eco, Position, On/Off)
      eco_selected = getValue(msg[1], 4, 4);
      status = getValue(msg[1], 6, 6);
      gear_position = getValue(msg[0], 4, 7);

    } else if (msgId == 0x5c0) { // Battery temperature
      if ( (msg[0]>>6) == 1 ) { // Checks that a value has been calculated, I think
        battery_temperature = msg[2] / 2 - 40;
      }
    
    } else if (msgId == 0x54c) { // Ambient temperature
      if (msg[6] != 0xff) { // Make sure it equals something
        ambient_temperature = msg[6] / 2.0 - 40;
      }
    }
  }
  if (serialEnabled) {
    unsigned long ms = millis();
    if (status) { 
      serialInterval = 50;
    } else { 
      serialInterval = 1000; // Lower the send rate if the car is off.
    }
    if (ms - serialLast > serialInterval) {
      serialLast = ms;
      Serial.print("on "); Serial.println(status);
      Serial.print("spd "); Serial.println(speed);
      Serial.print("kw "); Serial.println(battery_kw);
      Serial.print("soc "); Serial.println(soc);
      Serial.print("eco "); Serial.println(eco_selected);
      Serial.print("ac "); Serial.println(ac_fan_speed);
      Serial.print("gear "); Serial.println(gear_position);
      Serial.print("tmp_b "); Serial.println(battery_temperature);
      Serial.print("tmp_a "); Serial.println(ambient_temperature);
    }
  }
}

byte ArdLeaf::getValue(byte b, int pStart, int pEnd) {
  byte mask = 0;
  int counter = 1;
  for (int i = 0; i <= 7; i++) {
    if ( (i >= pStart) && (i <= pEnd) ) {
      mask += counter;
    }
    counter = counter * 2;
  }

  byte result = (b & mask);
  if (pStart != 0) {
    result = result >> pStart;
  }
  return result;
}

void ArdLeaf::printBinary(byte inByte, int len){
  for (int b = len-1; b >= 0; b--)
  {
    //Serial.println(i);
    Serial.print(bitRead(inByte, b));
  }
  Serial.println();
}

void ArdLeaf::setSerial(bool state) {
  serialEnabled = state;
}