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
    void setSerial(bool state);

    int speed;
    int status;
    int eco_selected;
    int gear_position;
    int soc_gids;
    float soc_percent;
    float soc;
    int soh;
    float battery_voltage;
    float battery_current;
    float battery_kw;
    float battery_temperature;
    float ambient_temperature;
    int ac_fan_speed;
  private:
    bool serialEnabled = true; // Serial enabled
    int serialInterval; // Interval of serial update
    long serialLast; // Stores the time of the last update

    int pinCS;
    int pinINT;
    long unsigned int msgId; // CAN message ID
    unsigned char msg[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    char out[128]; // Serial out
    
    MCP_CAN* canEV;

    byte getValue(byte b, int pStart, int pEnd);
    void printBinary(byte inByte, int len);
};

#endif
