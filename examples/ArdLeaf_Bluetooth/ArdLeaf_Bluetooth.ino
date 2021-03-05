#include <ArdLeaf.h>

ArdLeaf leaf(D8, D2); // cs, int

void setup() {
  leaf.startSerial(9600);
  leaf.startBluetooth(D4, D3);
  leaf.connect();
}

void loop() {
  leaf.update();
}
