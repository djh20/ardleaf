#ifndef _ARDLEAF_H_
#define _ARDLEAF_H_

#include "Arduino.h"
#include "metrics.h"
#include "mcp_can.h"
#include "gps.h"
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
    void startGPS(int tx, int rx);

  private:
    byte readByte(byte b, int pStart, int pEnd);
    void printBinary(byte inByte, int len);
    
  public:
    bool canEnabled;
    bool serialEnabled;
    bool bluetoothEnabled;
    bool gpsEnabled;
    
  private:
    MetricBool* powered;

    MetricInt* speed;
    MetricInt* left_speed;
    MetricInt* right_speed;

    MetricInt* gear;
    MetricBool* eco;
    MetricFloat* soc;
    MetricInt* gids;
    MetricFloat* energy;
    MetricBool* charging;
    MetricInt* climate_fan_speed;
    
    MetricFloat* inverter_temp;
    MetricFloat* motor_temp;

    MetricInt* trip_distance;

    int pinINT;

    long unsigned int msgId; // CAN message ID
    unsigned char msg[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    char out[128]; // Serial out
    unsigned long ms;

    MCP_CAN* canEV;
    TinyGPSPlus* gps;

    SoftwareSerial* gpsSerial;
    SoftwareSerial* bt;
    
    unsigned long gpsLastUpdate = 0;
    double gpsLastLatitude;
    double gpsLastLongitude;
    float tripDistance = 0;
};

#endif
