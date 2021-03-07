#include "Arduino.h"
#include "metrics.h"
#include <SoftwareSerial.h>

Metrics MyMetrics;

// metrics
Metrics::Metrics() {}

void Metrics::RegisterMetric(Metric* metric) {
  metric->id = metricCount;
  metricCount++;

  Serial.print(metric->name);
  Serial.print(" -> registered with id ");
  Serial.println(metric->id);

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

// metric
Metric::Metric(const char* n, int bytes) {
  name = n;
  byteCount = bytes;
  MyMetrics.RegisterMetric(this);
}
void Metric::send(SoftwareSerial* output) {}

// int
MetricInt::MetricInt(const char* n, int bytes) : Metric(n, bytes) {}

void MetricInt::setValue(int val) {
  if (value != val) { // check new value is different to current value
    value = val;

    Serial.print(name);
    Serial.print(" -> set to ");
    Serial.println(val);

    send(MyMetrics.output);
  }
}
void MetricInt::send(SoftwareSerial* output) {
  if (output == NULL) return;
  output->write(id);
  output->write((byte *) &value, byteCount);
}

// float
MetricFloat::MetricFloat(const char* n) : Metric(n, 2) {}

void MetricFloat::setValue(float val) {
  if (value != val) { // check new value is different to current value
    value = val;

    Serial.print(name);
    Serial.print(" -> set to ");
    Serial.println(val);

    send(MyMetrics.output);
  }
}
void MetricFloat::send(SoftwareSerial* output) {
  if (output == NULL) return;
  int valueInt = value*100;
  output->write(id);
  output->write((byte *) &valueInt, byteCount);
}

// bool
MetricBool::MetricBool(const char* n) : Metric(n, 1) {}

void MetricBool::setValue(bool val) {
  if (value != val) { // check new value is different to current value
    value = val;

    Serial.print(name);
    Serial.print(" -> set to ");
    Serial.println(val ? "true" : "false");

    send(MyMetrics.output);
  }
}
void MetricBool::send(SoftwareSerial* output) {
  if (output == NULL) return;
  output->write(id);
  output->write(value);
}