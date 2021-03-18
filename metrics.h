#ifndef _METRICS_H_
#define _METRICS_H_

#include "Arduino.h"
#include <SoftwareSerial.h>

class Metric {
  public:
    Metric(const char* n, int bytes);

  public:
    virtual void send(SoftwareSerial* output);
    virtual void log();

  public:
    Metric* next;
    const char* name;
    unsigned long lastUpdate = 0;
    int id;
    int byteCount;
};

class MetricInt: public Metric {
  public:
    MetricInt(const char* n, int bytes=1);

  public:
    void setValue(int val);
    void send(SoftwareSerial* output);
    void log();
    int value = 0;
};

class MetricFloat: public Metric {
  public:
    MetricFloat(const char* n);

  public:
    void setValue(float val);
    void send(SoftwareSerial* output);
    void log();
    float value = 0.0f;
};

class MetricBool: public Metric {
  public:
    MetricBool(const char* n);

  public:
    void setValue(bool val);
    void send(SoftwareSerial* bt);
    void log();
    bool value = false;
};

class Metrics {
  public:
    Metrics();

  public:
    void RegisterMetric(Metric* metric);
    void SendAll();
    void LogAll();
    //Metric* Find(const char* name);

  public:
    Metric* first;
    SoftwareSerial* output;
    bool serialEnabled = false;

  private:
    int metricCount = 0;
};

extern Metrics MyMetrics;

#endif