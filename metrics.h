#ifndef _METRICS_H_
#define _METRICS_H_

#include "Arduino.h"
#include <SoftwareSerial.h>

class MetricInt {
  public:
    MetricInt(int _id, int _byteCount);
    unsigned int value;
    int byteCount;
    void setValue(unsigned int newValue);
    void send(SoftwareSerial *s);
  private:
    int id;
    bool changed;
};

class MetricFloat {
  public:
    MetricFloat(int _id);
    float value;
    void setValue(float newValue);
    void send(SoftwareSerial *s);
  private:
    int id;
    bool changed;
};

class MetricBool {
  public:
    MetricBool(int _id);
    bool value;
    void setValue(bool newValue);
    void send(SoftwareSerial *s);
  private:
    int id;
    bool changed;
};

#endif