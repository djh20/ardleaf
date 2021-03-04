#include "Arduino.h"
#include "ArdLeaf.h"
#include "mcp_can.h"
#include <SPI.h>
#include <SoftwareSerial.h>

ArdLeaf::ArdLeaf(int pin_cs, int pin_int) {
  canEV = new MCP_CAN(pin_cs);
  pinMode(pin_int, INPUT);

  pinCS = pin_cs;
  pinINT = pin_int;
}

void ArdLeaf::connect() {
  Serial.print("Connecting to CAN...  ");
  
  if(canEV->begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("Connected!");
  } else {
    Serial.println("Error!");
  }

  canEV->setMode(MCP_NORMAL);
}

void ArdLeaf::update() {
  if( !digitalRead(pinINT) ) { // Check if data is available
    canEV->readMsgBuf(&msgId, &msgLen, msg);
    if (msgId == 0x5bc) { // SOC (With degradation)
      soc_gids = (msg[0] << 2) | (msg[1] >> 6);
      soc_percent = (soc_gids / MAX_GIDS) * 100.0F;
      soh = readByte(msg[4], 1, 7);

    } else if (msgId == 0x55b) { // SOC (Without degradation)
      soc = ( (msg[0] << 2) | (msg[1] >> 6) ) / 10.0F;

    } else if (msgId == 0x55a) { // Motor & inverter temperatures
      //motor_temperature = msg[5] * 0.5;
      //inverter_temperature = msg[1] * 0.5;

      // Motor, charge and inverter temperature guesses in Fahrenheit?
      motor_temperature = 5.0 / 9.0 * (msg[1] - 32);
      inverter_temperature = 5.0 / 9.0 * (msg[2] - 32);

    } else if (msgId == 0x1da) { // Motor RPM
      rpm = ( msg[4] << 7 | readByte(msg[5], 1, 7) );
      
    } else if (msgId == 0x1db) { // Voltage and current
      battery_voltage = ( (msg[2] << 2) | (msg[3] >> 6) ) / 2.0F;

      // sent by the LBC, measured inside the battery box
      // current is 11 bit twos complement big endian starting at bit 0
      int16_t current = ((int16_t) msg[0] << 3) | (msg[1] & 0xe0) >> 5;
      if (current & 0x0400) { // negative so extend the sign bit
        current |= 0xf800;
      }
      battery_current = -current / 2.0f;
      battery_power = (battery_current * battery_voltage)/1000.0F;
      
    } else if (msgId == 0x284) { // Speed sensors
      leftSpeed = (msg[0] << 8) | msg[1];
      rightSpeed = (msg[2] << 8) | msg[3];
      rearSpeed = (msg[4] << 8) | msg[5];

      speed = rearSpeed / 100;

    } else if (msgId == 0x54b) { // A/C
      ac_fan_speed = (msg[4] / 8);

    } else if (msgId == 0x11a) { // Shift controller (Eco, Position, On/Off)
      eco_selected = readByte(msg[1], 4, 4);
      status = readByte(msg[1], 6, 6);
      gear_position = readByte(msg[0], 4, 7);

    } else if (msgId == 0x5c0) { // Battery temperature
      if ( (msg[0]>>6) == 1 ) { // Checks that a value has been calculated, I think
        battery_temperature = msg[2] / 2 - 40;
      }
    
    } else if (msgId == 0x54c) { // Ambient temperature
      if (msg[6] != 0xff) { // Make sure it equals something
        ambient_temperature = msg[6] / 2.0 - 40;
      }

    } else if (msgId == 0x1d4) {
      int chargeStatus = readByte(msg[6], 5, 7);
      if (chargeStatus == 6 || chargeStatus == 7) {
        charging = 1;
      } else {
        charging = 0;
      }

    }
  }
  if (serialEnabled) {
    unsigned long ms = millis();
    if (ms - serialLast > serialInterval) {
      serialLast = ms;

      //Serial.print(status); Serial.print(','); Serial.print(speed); Serial.print(','); Serial.println(ac_fan_speed);
      Serial.print(speed); Serial.print(',');
      Serial.print(rearSpeed); Serial.print(',');
      Serial.print(leftSpeed); Serial.print(',');
      Serial.print(rightSpeed); Serial.print(',');
      Serial.print(soc); Serial.print(',');
      Serial.print(soc_gids); Serial.print(',');
      Serial.print(rpm); Serial.print(',');
      Serial.print(battery_power); Serial.print(',');
      Serial.print(battery_temperature); Serial.print(',');
      Serial.print(motor_temperature); Serial.print(',');
      Serial.println(inverter_temperature);
      
      /*
      Serial.print("on "); Serial.println(status);
      Serial.print("spd "); Serial.println(speed);
      Serial.print("kw "); Serial.println(battery_power);
      Serial.print("soc "); Serial.println(soc);
      Serial.print("eco "); Serial.println(eco_selected);
      Serial.print("ac "); Serial.println(ac_fan_speed);
      Serial.print("gear "); Serial.println(gear_position);
      Serial.print("tmp_b "); Serial.println(battery_temperature);
      Serial.print("tmp_a "); Serial.println(ambient_temperature);
      Serial.print("charging "); Serial.println(charging);
      */
    }
  }
}

byte ArdLeaf::readByte(byte b, int pStart, int pEnd) {
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