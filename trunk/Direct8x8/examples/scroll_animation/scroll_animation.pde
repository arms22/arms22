#include <Direct8x8.h>

/*
  Scroll Animation
  by arms22 <http://arms22.blog91.fc2.com/>

  Demonstrates the use of the Direct8x8 library
  
  pin 0- 7: row0-row7
  pin 8-12,14-16: row8-row12, row13-row15
*/

#include <Sprite.h>
#include <Metro.h>

Direct8x8 myMatrix = Direct8x8( 0, 1, 2, 3, 4, 5, 6, 7,
                                8, 9,10,11,12,14,15,16);

Sprite scroll_image = Sprite(
  8, 8,
  B00011110,
  B00110110,
  B00100110,
  B01100110,
  B01111110,
  B01111110,
  B01100110,
  B01100110
);

Metro scroll_event = Metro(300);

int x;

void setup()
{
  x = 8;
}

void loop()
{
  if(scroll_event.check()){
    myMatrix.clear();
    myMatrix.write(x, 0, scroll_image);
    x--;
    if(x <= -scroll_image.width()) x = 8;
  }
  myMatrix.vsync();
}
