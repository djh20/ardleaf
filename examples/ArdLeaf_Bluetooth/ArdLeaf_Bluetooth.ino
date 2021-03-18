#include <ArdLeaf.h>

ArdLeaf leaf;

void setup() {
  leaf.startSerial(115200);
  leaf.startCAN(D8, D2); // CS, INT (pins)
  leaf.startBluetooth(D4, D3); // TX, RX
  leaf.begin();
}

void loop() {
  leaf.update();
}
