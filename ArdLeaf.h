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
	  int rearSpeed;
    int leftSpeed;
    int rightSpeed;
    int status;
    int eco_selected;
    int charging;
    int gear_position;
    int soc_gids;
    int soh;
    int rpm;

    int ac_fan_speed;

    float soc_percent;
    float soc;
    
    float battery_voltage;
    float battery_current;
    float battery_power;

    float battery_temperature;
    float ambient_temperature;
    float motor_temperature;
    float inverter_temperature;

  private:
    bool serialEnabled = true; // Serial enabled
    int serialInterval = 500; // Interval of serial update
    long serialLast; // Stores the time of the last update

    int pinCS;
    int pinINT;
    long unsigned int msgId; // CAN message ID
    unsigned char msg[8]; // CAN 8 byte buffer
    unsigned char msgLen = 0;  // CAN data length
    char out[128]; // Serial out
    
    MCP_CAN* canEV;

    byte readByte(byte b, int pStart, int pEnd);
    void printBinary(byte inByte, int len);
};

#endif
