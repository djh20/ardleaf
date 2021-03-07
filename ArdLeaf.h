#ifndef _ARDLEAF_H_
#define _ARDLEAF_H_

#include "Arduino.h"
#include "mcp_can.h"
#include "metrics.h"
#include <SPI.h>
#include <SoftwareSerial.h>

class ArdLeaf
{
  public:
    ArdLeaf();

  public:
    void begin();
    void update();
    void startCAN(int pin_cs, int pin_int);
    void startSerial(long baud);
    void startBluetooth(int tx, int rx);

  private:
    byte readByte(byte b, int pStart, int pEnd);
    void printBinary(byte inByte, int len);
    
  public:
    bool canEnabled;
    bool serialEnabled;
    bool bluetoothEnabled;
    
  private:
    MetricBool* powered;
    MetricInt* speed;
    MetricInt* gear;
    MetricBool* eco;
    MetricFloat* soc;
    MetricFloat* energy;
    MetricBool* charging;
    MetricInt* climate_fan_speed;

    int pinINT;

    long unsigned int msgId; // CAN message ID
    unsigned char msg[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    char out[128]; // Serial out

    MCP_CAN* canEV;
    SoftwareSerial* bt;
};

#endif
