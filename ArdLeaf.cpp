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
  Serial.println("--- ArdLeaf ---");
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

    } else if (msgId == 0x55b) { // SOC (Without degradation)
      soc_displayed = ( (msg[0] << 2) | (msg[1] >> 6) ) / 10.0F;

    } else if (msgId == 0x54b) { // A/C
      ac_fan_speed = (msg[4] / 8);

    } else if (msgId == 0x11a) { // Shift controller (Eco, Position, On/Off)
      eco_mode = getValue(msg[1], 4, 4);

    }
  }
}

byte ArdLeaf::getValue(byte b, int pStart, int pEnd) {
  //printBinary(b);
  
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