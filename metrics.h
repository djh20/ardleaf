#ifndef _METRICS_H_
#define _METRICS_H_

#include "Arduino.h"
#include <SoftwareSerial.h>

class Metric {
  public:
    Metric(const char* n, int bytes);

  public:
    Metric* next;
    const char* name;
    int id;
    int byteCount;
};

class MetricInt: public Metric {
  public:
    MetricInt(const char* n, int bytes=1);

  public:
    void setValue(int val);
    void send(SoftwareSerial* output);
    int value = 0;
};

class MetricFloat: public Metric {
  public:
    MetricFloat(const char* n);

  public:
    void setValue(float val);
    void send(SoftwareSerial* output);
    float value = 0.0f;
};

class MetricBool: public Metric {
  public:
    MetricBool(const char* n);

  public:
    void setValue(bool val);
    void send(SoftwareSerial* bt);
    bool value = false;
};

class Metrics {
  public:
    Metrics();

  public:
    void RegisterMetric(Metric* metric);
    //Metric* Find(const char* name);

  public:
    Metric* first;
    SoftwareSerial* output;

  private:
    int metricCount = 0;
};

extern Metrics MyMetrics;

#endif