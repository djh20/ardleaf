#include <ArdLeaf.h>

long last = 0;
long interval = 1000;

ArdLeaf leaf(10, 2); // CS, INT

void setup() {
  Serial.begin(115200);
  leaf.connect();
}

void loop() {
  leaf.update();

  unsigned long ms = millis();
  if (ms - last > interval) {
    last = ms;
    Serial.println(leaf.ac_fan_speed);
  }
}
