#include <ArdLeaf.h>

ArdLeaf leaf(D8, D2); // CS, INT

void setup() {
  leaf.startSerial(9600);
  leaf.connect();
}

void loop() {
  leaf.update();
}
