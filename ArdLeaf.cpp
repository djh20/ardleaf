#include "Arduino.h"
#include "ArdLeaf.h"
#include "mcp_can.h"
#include "metrics.h"
#include <SPI.h>
#include <SoftwareSerial.h>

ArdLeaf::ArdLeaf() {
  
  /*
  powered = new MetricBool(1);
  speed = new MetricInt(2, 1);
  gear = new MetricInt(3, 1);
  soc = new MetricFloat(4);
  energy = new MetricFloat(5);
  */
}

void ArdLeaf::begin() {
  powered = new MetricBool("powered");
  speed = new MetricInt("speed");
  gear = new MetricInt("gear");
  soc = new MetricFloat("soc");
  energy = new MetricFloat("energy");
}

void ArdLeaf::startCAN(int pin_cs, int pin_int) {
  Serial.print("connecting to can...  ");

  canEV = new MCP_CAN(pin_cs);
  pinMode(pin_int, INPUT);
  
  if(canEV->begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("success :)");
    canEV->setMode(MCP_NORMAL);
    pinINT = pin_int;
    canEnabled = true;
  } else {
    Serial.println("error!");
  }
}

void ArdLeaf::startSerial(long baud) {
  Serial.begin(baud);
  serialEnabled = true;
}

void ArdLeaf::startBluetooth(int tx, int rx) {
  bt = new SoftwareSerial(tx, rx);
  bt->begin(9600);
  bluetoothEnabled = true;

  MyMetrics.output = bt;
}

void ArdLeaf::update() {
  
  if (Serial.available() > 0) {
    int input = Serial.read();
    //counter++;

    powered->setValue(true);
    speed->setValue(24);
    gear->setValue(4);
    soc->setValue(50.7f);
    energy->setValue(76.3f);

    //speed->setValue(speed->value+1);


    //speed->setValue(speed->value+1);
    //Serial.println(input, DEC);
    //soc->setValue(soc->value+0.5f);
    //soc->send(bt);
  }
  
  
  if( canEnabled && !digitalRead(pinINT) ) { // Check if data is available
    canEV->readMsgBuf(&msgId, &msgLen, msg);
    if (msgId == 0x5bc) { // SOC (With degradation)
      ///soc_gids = (msg[0] << 2) | (msg[1] >> 6);
      ///soc_gids_percent = (soc_gids / MAX_GIDS) * 100.0F;
      ///soh = readByte(msg[4], 1, 7);

    } else if (msgId == 0x55b) { // SOC (Without degradation)
      float soc_percent = ( (msg[0] << 2) | (msg[1] >> 6) ) / 10.0F;

      /*
      soc->setValue(soc_percent);
      if (bluetoothEnabled) {
        soc->send(bt);
      }
      */

    } else if (msgId == 0x55a) { // Motor & inverter temperatures
      // Motor, charge and inverter temperature guesses in Fahrenheit?
      ///motor_temperature = 5.0 / 9.0 * (msg[1] - 32);
      ///inverter_temperature = 5.0 / 9.0 * (msg[2] - 32);

    } else if (msgId == 0x1da) { // Motor RPM
      ///rpm = ( msg[4] << 7 | readByte(msg[5], 1, 7) );
      
    } else if (msgId == 0x1db) { // Voltage and current
      ///battery_voltage = ( (msg[2] << 2) | (msg[3] >> 6) ) / 2.0F;

      // sent by the LBC, measured inside the battery box
      // current is 11 bit twos complement big endian starting at bit 0

      /*
      int16_t current = ((int16_t) msg[0] << 3) | (msg[1] & 0xe0) >> 5;
      if (current & 0x0400) { // negative so extend the sign bit
        current |= 0xf800;
      }
      battery_current = -current / 2.0f;
      battery_power = (battery_current * battery_voltage)/1000.0F;
      */
      
    } else if (msgId == 0x284) { // Speed sensors
      ///leftSpeed = (msg[0] << 8) | msg[1];
      ///rightSpeed = (msg[2] << 8) | msg[3];
      unsigned int rearSpeed = (msg[4] << 8) | msg[5];

      /*
      speed->setValue(rearSpeed / 100);
      if (bluetoothEnabled) {
        speed->send(bt);
      }
      */

    } else if (msgId == 0x54b) { // A/C
      ///ac_fan_speed = (msg[4] / 8);

    } else if (msgId == 0x11a) { // Shift controller (Eco, Position, On/Off)
      ///eco_selected = readByte(msg[1], 4, 4);
      ///status = readByte(msg[1], 6, 6);
      ///gear_position = readByte(msg[0], 4, 7);

      bool status = readByte(msg[1], 6, 6);

      /*
      powered->setValue(status);
      if (bluetoothEnabled) {
        powered->send(bt);
      }
      */

    } else if (msgId == 0x5c0) { // Battery temperature
      if ( (msg[0]>>6) == 1 ) { // Checks that a value has been calculated, I think
        ///battery_temperature = msg[2] / 2 - 40;
      }
    
    } else if (msgId == 0x54c) { // Ambient temperature
      if (msg[6] != 0xff) { // Make sure it equals something
        ///ambient_temperature = msg[6] / 2.0 - 40;
      }

    } else if (msgId == 0x1d4) {
      /*
      int chargeStatus = readByte(msg[6], 5, 7);
      if (chargeStatus == 6 || chargeStatus == 7) {
        charging = 1;
      } else {
        charging = 0;
      }
      */
    }
  }
  /*
  if (bluetoothEnabled) {
    ms = millis();

    if (ms-lastms > 100) {
      lastms = ms;
      sendCounter++;

      bt.print(speed); bt.print(',');
      bt.print(battery_power);
      
      if (sendCounter >= 10) {
        sendCounter = 0;
        bt.print(',');
        bt.print(status); bt.print(',');
        bt.print(gear_position); bt.print(',');
        bt.print(soc);
      } else {
        bt.print(',');
      }

      bt.println();
    }
  }
  */
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