#include <Direct8x8.h>

/*
  Flip Animation
  by arms22 <http://arms22.blog91.fc2.com/>

  Demonstrates the use of the Direct8x8 library
  
  pin 0- 7: row0-row7
  pin 8-12,14-16: row8-row12, row13-row15
*/

#include <Sprite.h>
#include <Metro.h>

Direct8x8 myMatrix = Direct8x8( 0, 1, 2, 3, 4, 5, 6, 7,
                                8, 9,10,11,12,14,15,16);

Sprite one = Sprite(
  8, 5,
  B01000010,
  B00000000,
  B00000000,
  B01111110,
  B00000000
);

Sprite two = Sprite(
  8, 5,
  B01000010,
  B00000000,
  B01111110,
  B01000010,
  B01111110
);

Metro flip_event = Metro(1000);

void setup()
{
}

int which_sprite = 1;

void loop()
{
  if(flip_event.check()){
    if(which_sprite == 1){
      myMatrix.write(0,2,one);
      which_sprite = 2;
    }else{
      myMatrix.write(0,2,two);
      which_sprite = 1;
    }
  }
  myMatrix.vsync();
}
