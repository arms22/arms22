#include <Direct8x8.h>
#include <Sprite.h>
#include <Metro.h>

Direct8x8 myMatrix = Direct8x8( 0, 1, 2, 3, 4, 5, 6, 7,
                                8, 9,10,11,12,14,15,16);

Sprite wave = Sprite(
  8, 4,
  B00011000,
  B00100100,
  B01000010,
  B10000001
);

Metro scroll_event = Metro(100);

void setup()
{
}

int x = 0;

void loop()
{
  if(scroll_event.check()){
    myMatrix.write(x, 2, wave);
    myMatrix.write(x - 8, 2, wave);
    x++;
    if(x == 8) x = 0;
  }
  myMatrix.vsync();
}
