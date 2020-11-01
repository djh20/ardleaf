#ifndef ArdLeaf_h
#define ArdLeaf_h

#include "Arduino.h"
#include "mcp_can.h"

class ArdLeaf
{
  public:
    ArdLeaf(int pinCS, int pinINT);
    void connect();
    void update();
  private:
    int pinCS;
    int pinINT;
    MCP_CAN* canEV;
};

#endif
