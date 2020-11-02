#include "Arduino.h"
#include "ArdLeaf.h"
#include "mcp_can.h"
#include <SPI.h>

ArdLeaf::ArdLeaf(int pin_cs, int pin_int)
{
  pinMode(pin_int, INPUT);
  MCP_CAN canEV(pin_cs);

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
  
}

void ArdLeaf::update() {
  if( !digitalRead(pinINT) ) { // Check if data is available
    canEV->readMsgBuf(&msgId, &msgLen, msgBuf);

    if (msgId == 0x54b) {
      ac_fan_speed = getValue(msgBuf[4], 0, 7) / 8;
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