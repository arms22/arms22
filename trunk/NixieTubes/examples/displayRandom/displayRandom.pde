#include <NixieTubes.h>

NixieTubes randomTubes(7,8,9,10,11,12,6,4,3,5);

void setup() {
  randomTubes.begin();
}

void loop() {
  static unsigned long last_time = 0;
  if(millis() - last_time > 100){
    unsigned long w,t = random();
    w = t % 1000000;
    randomTubes.write(0, w / 100000);
    w = t % 100000;
    randomTubes.write(1, w / 10000);
    w = t % 10000;
    randomTubes.write(2, w / 1000);
    w = t % 1000;
    randomTubes.write(3, w / 100);
    w = t % 100;
    randomTubes.write(4, w / 10);
    w = t % 10;
    randomTubes.write(5, w);
	last_time = millis();
  }
  randomTubes.update();
  delay(1);
}
