#include "Arduino.h"
#include "metrics.h"
#include <SoftwareSerial.h>

Metrics MyMetrics;

// metrics
Metrics::Metrics() {}

void Metrics::RegisterMetric(Metric* metric) {
  metric->id = metricCount;
  metricCount++;

  if (serialEnabled) {
    Serial.print("> Registered metric ["); 
    Serial.print(metric->name); 
    Serial.println("]");
  }

  if (first == NULL) {
    first = metric;
    return;
  }

  for (Metric* m = first; m != NULL; m=m->next) {
    if (m->next == NULL) {
      m->next = metric;
      return;
    }
  }
}
void Metrics::SendAll() {
  for (Metric* m = first; m != NULL; m=m->next) {
    m->send(output);
  }
}
void Metrics::LogAll() {
  for (Metric* m = first; m != NULL; m=m->next) {
    m->log();
  }
}

// metric
Metric::Metric(const char* n, int bytes) {
  name = n;
  byteCount = bytes;
  MyMetrics.RegisterMetric(this);
}
void Metric::send(SoftwareSerial* output) {}
void Metric::log() {}

// int
MetricInt::MetricInt(const char* n, int bytes) : Metric(n, bytes) {}

void MetricInt::setValue(int val) {
  lastUpdate = millis();
  if (value != val) { // check new value is different to current value
    value = val;
    send(MyMetrics.output);
  }
}
void MetricInt::send(SoftwareSerial* output) {
  if (output == NULL) return;
  output->write(id);
  output->write((byte *) &value, byteCount);
}
void MetricInt::log() {
  Serial.print(name);
  Serial.print(" [");
  Serial.print(value);
  Serial.println("]");
}

// float
MetricFloat::MetricFloat(const char* n) : Metric(n, 2) {}

void MetricFloat::setValue(float val) {
  lastUpdate = millis();
  if (value != val) { // check new value is different to current value
    value = val;
    send(MyMetrics.output);
  }
}
void MetricFloat::send(SoftwareSerial* output) {
  if (output == NULL) return;
  int valueInt = value*100;
  output->write(id);
  output->write((byte *) &valueInt, byteCount);
}
void MetricFloat::log() {
  Serial.print(name);
  Serial.print(" [");
  Serial.print(value);
  Serial.println("]");
}

// bool
MetricBool::MetricBool(const char* n) : Metric(n, 1) {}

void MetricBool::setValue(bool val) {
  lastUpdate = millis();
  if (value != val) { // check new value is different to current value
    value = val;
    send(MyMetrics.output);
  }
}
void MetricBool::send(SoftwareSerial* output) {
  if (output == NULL) return;
  output->write(id);
  output->write(value);
}
void MetricBool::log() {
  Serial.print(name);
  Serial.print(" [");
  Serial.print(value ? "true" : "false");
  Serial.println("]");
}