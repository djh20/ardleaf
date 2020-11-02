#ifndef ArdLeaf_h
#define ArdLeaf_h

#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

class ArdLeaf
{
  public:
    ArdLeaf(int pin_cs, int pin_int);
    
    void connect();
    void update();

    int ac_fan_speed;
  private:
    int pinCS;
    int pinINT;
    long unsigned int msgId; // CAN message ID
    unsigned char msgBuf[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    
    MCP_CAN* canEV;

    byte getValue(byte b, int pStart, int pEnd);
};

#endif
