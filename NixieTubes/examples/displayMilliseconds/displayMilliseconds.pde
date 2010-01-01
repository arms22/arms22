#include <NixieTubes.h>

NixieTubes millisTubes(7,8,9,10,11,12,6,4,3,5);

void setup() {
  millisTubes.begin();
}

void loop() {
  unsigned long w,t = millis();
  w = t % 1000000;
  millisTubes.write(0, w / 100000);
  w = t % 100000;
  millisTubes.write(1, w / 10000);
  w = t % 10000;
  millisTubes.write(2, w / 1000);
  w = t % 1000;
  millisTubes.write(3, w / 100);
  w = t % 100;
  millisTubes.write(4, w / 10);
  w = t % 10;
  millisTubes.write(5, w);
  millisTubes.update();
  delay(1);
}
