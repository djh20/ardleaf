#include <ArdLeaf.h>

ArdLeaf leaf(10, 2); // CS, INT

void setup() {
  Serial.begin(115200);
  leaf.connect();
}

void loop() {
  leaf.update();
}
