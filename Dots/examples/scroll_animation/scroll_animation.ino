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

Dots myDots = Dots();

const byte scroll_image[] = {
  B00000000,
  B01111110,
  B00000000,
  B00000000,
  B00000000,
  B01111100,
  B00010010,
  B00010010,
  B01111100,
  B00000000,
  B01111110,
  B00000100,
  B00000100,
  B01111110,
  B00000000,
  B01110000,
  B01001000,
  B01101000,
  B00111000,
  B00000000,
  B01111110, 
  B01000010, 
  B01000010, 
  B00111100, 
  B00000000, 
  B00111100, 
  B01000010, 
  B01000010, 
  B00111100, 
  B00000000, 
  B00000010, 
  B01111110,
  B00000010, 
  B00000000, 
  B00001000, 
  B01010100, 
  B01010100, 
  B00100000,
};

int img_h = sizeof(scroll_image) / sizeof(scroll_image[0]);
int wnd_y;

void setup()
{
  wnd_y = -8;
  myDots.begin();
}

void loop()
{
  myDots.clear();

  if(wnd_y < 0){
    myDots.write(-wnd_y, &scroll_image[0], 8 + wnd_y);
  }
  else if(wnd_y < (img_h - 8)){
    myDots.write(0, &scroll_image[wnd_y], 8);
  }
  else{
    myDots.write(0, &scroll_image[wnd_y], img_h - wnd_y);
  }

  wnd_y++;
  if(wnd_y >= img_h){
    wnd_y = -8;
  }

  delay(100);
}

