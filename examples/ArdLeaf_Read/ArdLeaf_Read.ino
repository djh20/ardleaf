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
    Serial.println("\n--- Info ---");
    Serial.println("== Car ==");
    Serial.print("Eco mode: "); Serial.println(leaf.eco_mode);
    Serial.println("\n== SOC ==");
    Serial.print("Gids: "); Serial.println(leaf.soc_gids);
    Serial.print("Percent: "); Serial.println(leaf.soc_percent);
    Serial.print("Displayed: "); Serial.println(leaf.soc_displayed);
    Serial.println("\n== A/C ==");
    Serial.print("Fan Speed: "); Serial.println(leaf.ac_fan_speed);
    Serial.println("-------------------");
  }
}
