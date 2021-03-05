#include "Arduino.h"
#include "metrics.h"
#include <SoftwareSerial.h>

MetricInt::MetricInt(int _id, int _byteCount) {
  id = _id;
  byteCount = _byteCount;
  value = 0;
}
void MetricInt::setValue(unsigned int newValue) {
  if (newValue != value) {
    Serial.print(id); Serial.print(' -> '); Serial.println(value);
    value = newValue;
    changed = true;
  }
}
void MetricInt::send(SoftwareSerial *s) {
  if (changed) {
    s->write(id);
    s->write((byte *) &value, byteCount);
    changed = false;
  }
}

MetricFloat::MetricFloat(int _id) {
  id = _id;
  value = 0.0f;
}
void MetricFloat::setValue(float newValue) {
  if (newValue != value) {
    Serial.print(id); Serial.print(' -> '); Serial.println(value);
    value = newValue;
    changed = true;
  }
}
void MetricFloat::send(SoftwareSerial *s) {
  if (changed) {
    int valueInt = value*100;
    s->write(id);
    s->write((byte *) &valueInt, 2);
    changed = false;
  }
}

MetricBool::MetricBool(int _id) {
  id = _id;
  value = false;
}
void MetricBool::setValue(bool newValue) {
  if (newValue != value) {
    Serial.print(id); Serial.print(' -> '); Serial.println(value);
    value = newValue;
    changed = true;
  }
}
void MetricBool::send(SoftwareSerial *s) {
  if (changed) {
    s->write(id);
    s->write(value);
    changed = false;
  }
}