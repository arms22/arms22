/*** Scroll Animation
 * by arms22 <http://arms22.blog91.fc2.com/>
 * 
 * Demonstrates the use of the Dots library
 * 
 * Matrix  Arduino
 * 1  R5 -> 17(analog 3)
 * 2  R7 -> 16(analog 2)
 * 3  C2 -> 15(analog 1)
 * 4  C3 -> 14(analog 0)
 * 5  R8 -> 13
 * 6  C5 -> 12
 * 7  R6 -> 11
 * 8  R3 -> 10
 * 9  R1 -> 9
 * 10 C4 -> 8
 * 11 C6 -> 7
 * 12 R4 -> 6
 * 13 C1 -> 5
 * 14 R2 -> 4
 * 15 C7 -> 3
 * 16 C8 -> 2
 */

#include <Dots.h>

Dots myDots = Dots(9,4,10,6,17,11,16,13,5,15,14,8,12,7,3,2);

const byte scroll_image[] = {
  B00001100,
  B00011110,
  B00111110,
  B01111100,
  B00111110,
  B00011110,
  B00001100,
  B00000000,
};

int y;

void setup()
{
  y = 0;
  myDots.begin();
}

void loop()
{
  myDots.clear();
  if(y < 8){
    myDots.write(0, &scroll_image[8-y], y);
  }
  else if(y < 16){
    myDots.write(y-8, scroll_image, 16-y);
  }
  else{
    y = 0;
  }
  y++;
  myDots.update(12);
}

