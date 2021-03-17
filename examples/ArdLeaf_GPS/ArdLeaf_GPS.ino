#include <ArdLeaf.h>

ArdLeaf leaf;

void setup() {
  leaf.startSerial(9600);
  leaf.startCAN(D8, D2); // CS, INT (pins)
  leaf.startBluetooth(D4, D3); // TX, RX
  leaf.startGPS(D1, D0); // TX, RX
  leaf.begin();
}

void loop() {
  leaf.update();
}
