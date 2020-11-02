#ifndef ArdLeaf_h
#define ArdLeaf_h

#include "Arduino.h"
#include "mcp_can.h"
#include <SPI.h>

#define MAX_GIDS 281.0F

class ArdLeaf
{
  public:
    ArdLeaf(int pin_cs, int pin_int);
    
    void connect();
    void update();

    int ac_fan_speed;
    int soc_gids;
    int eco_mode;
    float soc_percent;
    float soc_displayed;
  private:
    int pinCS;
    int pinINT;
    long unsigned int msgId; // CAN message ID
    unsigned char msg[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    
    MCP_CAN* canEV;

    byte getValue(byte b, int pStart, int pEnd);
};

#endif
