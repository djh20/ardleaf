#include <ArdLeaf.h>

long last = 0;
long interval = 450;

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
    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println("-------- Info --------");
    
    Serial.println(" Car");
    Serial.print("Status: "); Serial.println(leaf.status);
    Serial.print("Speed: "); Serial.print(leaf.speed); Serial.println(" km/h");
    Serial.print("Eco mode: "); Serial.println(leaf.eco_selected);
    Serial.print("Gear: "); Serial.println(leaf.gear_position);
    Serial.println("\n SOC/SOH");
    Serial.print("Gids: "); Serial.println(leaf.soc_gids);
    Serial.print("Percent: "); Serial.print(leaf.soc_percent); Serial.println("%");
    Serial.print("Displayed: "); Serial.print(leaf.soc_displayed); Serial.println("%");
    Serial.print("SOH: "); Serial.print(leaf.soh); Serial.println("%");
    Serial.println("\n Battery");
    Serial.print(leaf.battery_volts); Serial.println(" V");
    Serial.print(leaf.battery_current); Serial.println(" A");
    Serial.print(leaf.battery_kw); Serial.println(" kW");
    Serial.print(leaf.battery_temperature); Serial.println(" °C");
    Serial.println("\n Other");
    Serial.print("A/C Speed: "); Serial.println(leaf.ac_fan_speed);
    Serial.print("Ambient Temperature: "); Serial.print(leaf.ambient_temperature); Serial.println(" °C");
    Serial.println("----------------------");
  }
  
  
}
