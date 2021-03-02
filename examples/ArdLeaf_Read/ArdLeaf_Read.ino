#include <ArdLeaf.h>

ArdLeaf leaf(D8, D2); // CS, INT

void setup() {
  Serial.begin(115200);
  leaf.connect();
}

void loop() {
  leaf.update();
}
