#include <ArdLeaf.h>

long last = 0;
long interval = 1000;

ArdLeaf leaf(10, 2); // INT, CS

void setup() {
  Serial.begin(115200);
  delay(3000);
  leaf.connect();
}

void loop() {
  leaf.update();

  unsigned long ms = millis();
  if (ms - last > interval) {
    last = ms;
    Serial.println("-- Nissan Leaf --");
    Serial.print("Fan speed: "); Serial.println(leaf.ac_fan_speed);
  }
}
