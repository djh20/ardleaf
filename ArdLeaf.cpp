#include "Arduino.h"
#include "ArdLeaf.h"
#include "mcp_can.h"
#include "metrics.h"
#include <SPI.h>
#include <SoftwareSerial.h>

ArdLeaf::ArdLeaf() {}

void ArdLeaf::begin() {
  powered = new MetricBool("powered");

  speed = new MetricInt("speed");
  left_speed = new MetricInt("left_speed", 2);
  right_speed = new MetricInt("right_speed", 2);

  gear = new MetricInt("gear");
  eco = new MetricBool("eco");
  soc = new MetricFloat("soc");
  gids = new MetricInt("gids");
  energy = new MetricFloat("energy");
  charging = new MetricBool("charging");
  climate_fan_speed = new MetricInt("climate_fan_speed");

  inverter_temp = new MetricFloat("inverter_temp");
  motor_temp = new MetricFloat("motor_temp");
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
    if (input == 97) { // (lowercase a) command to send all metrics
      MyMetrics.SendAll();
    }
  }

  if (bluetoothEnabled && bt->available() > 0) {
    int data = bt->read();
    if (data == 1) { // command 1 (send all metrics)
      MyMetrics.SendAll();
    }
  }
  
  if( canEnabled && !digitalRead(pinINT) ) { // Check if data is available
    ms = millis();
    canEV->readMsgBuf(&msgId, &msgLen, msg);
    if (msgId == 0x5bc) { // SOC (With degradation)
      int soc_gids = (msg[0] << 2) | (msg[1] >> 6);

      if (soc_gids < 1000) { // gids shows incorrect value of 1023 during startup
        gids->setValue(soc_gids);
      }
      ///soc_gids_percent = (soc_gids / MAX_GIDS) * 100.0F;
      ///soh = readByte(msg[4], 1, 7);

    } else if (msgId == 0x55b) { // SOC (Without degradation)
      float soc_percent = ( (msg[0] << 2) | (msg[1] >> 6) ) / 10.0F;
      
      soc->setValue(soc_percent);

    } else if (msgId == 0x55a) { // Motor & inverter temperatures
      // Motor, charge and inverter temperature guesses in Fahrenheit?
      float motor = 5.0 / 9.0 * (msg[1] - 32);
      float inverter = 5.0 / 9.0 * (msg[2] - 32);

      motor_temp->setValue(motor);
      inverter_temp->setValue(inverter);

    } else if (msgId == 0x1da) { // Motor RPM
      ///rpm = ( msg[4] << 7 | readByte(msg[5], 1, 7) );
      
    } else if (msgId == 0x1db) { // Voltage and current
      float battery_voltage = ( (msg[2] << 2) | (msg[3] >> 6) ) / 2.0F;

      // sent by the LBC, measured inside the battery box
      // current is 11 bit twos complement big endian starting at bit 0

      int16_t current = ((int16_t) msg[0] << 3) | (msg[1] & 0xe0) >> 5;
      if (current & 0x0400) { // negative so extend the sign bit
        current |= 0xf800;
      }
      float battery_current = -current / 2.0f;
      float battery_power = (battery_current * battery_voltage)/1000.0F;
      
      if (powered->value && ms-80 >= energy->lastUpdate) { // Only set if the car is on and it's been atleast 80ms since last update
        energy->setValue(battery_power);
      }
      
    } else if (msgId == 0x284) { // Speed sensors
      unsigned int leftSpeed = (msg[0] << 8) | msg[1];
      unsigned int rightSpeed = (msg[2] << 8) | msg[3];
      unsigned int rearSpeed = (msg[4] << 8) | msg[5];

      speed->setValue(rearSpeed / 100);

      if (ms-100 >= left_speed->lastUpdate) left_speed->setValue(leftSpeed / 10);
      if (ms-100 >= right_speed->lastUpdate) right_speed->setValue(rightSpeed / 10);

      // divide by 208 for correct speed

    } else if (msgId == 0x54b) { // Climate
      int fanSpeed = (msg[4] / 8);

      climate_fan_speed->setValue(fanSpeed);

    } else if (msgId == 0x11a) { // Shift controller (Eco, Gear, On/Off)
      bool status = readByte(msg[1], 6, 6);
      bool ecoSelected = readByte(msg[1], 4, 4);
      int gearPosition = readByte(msg[0], 4, 7);
      
      powered->setValue(status);
      gear->setValue(gearPosition);
      eco->setValue(ecoSelected);

    } else if (msgId == 0x5c0) { // Battery temperature
      if ( (msg[0]>>6) == 1 ) { // Checks that a value has been calculated, I think
        ///battery_temperature = msg[2] / 2 - 40;
      }
    
    } else if (msgId == 0x54c) { // Ambient temperature
      if (msg[6] != 0xff) { // Make sure it equals something
        ///ambient_temperature = msg[6] / 2.0 - 40;
      }

    } else if (msgId == 0x1d4) {
      int chargeValue = readByte(msg[6], 5, 7);
      bool chargeStatus = (chargeValue == 6 || chargeValue == 7) ? true : false;

      charging->setValue(chargeStatus);
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