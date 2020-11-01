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
  Serial.println("--- ArdLeaf Development Version ---");
  Serial.print("Connecting to CAN...  ");
  
  if(canEV->begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("Connected!");
  } else {
    Serial.println("Error!");
  }
  
}

void ArdLeaf::update() {
  Serial.println(pinCS);
}
