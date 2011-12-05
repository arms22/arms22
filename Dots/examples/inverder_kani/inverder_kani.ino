/*** inverder kani
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

const byte inv_a[] = {
  B00001110,
  B00011000,
  B10111110,
  B01101101,
  B00111101,
  B00111100,
  B00111101,
  B01101101,
  B10111110,
  B00011000,
  B00001110,
};

const byte inv_b[] = {
  B01111000,
  B00011101,
  B10111110,
  B01101100,
  B00111100,
  B00111100,
  B00111100,
  B01101100,
  B10111110,
  B00011101,
  B01111000,
};

int img_h = 11;
int wnd_y;
const byte *pattern;

void setup()
{
  wnd_y = -8;
  pattern = inv_a;
  myDots.begin();
}

void loop()
{
  myDots.clear();
  
  if(wnd_y < 0){
    myDots.write(-wnd_y, &pattern[0], 8 + wnd_y);
  }
  else if(wnd_y < (img_h - 8)){
    myDots.write(0, &pattern[wnd_y], 8);
  }
  else{
    myDots.write(0, &pattern[wnd_y], img_h - wnd_y);
  }
  
  if(wnd_y < img_h-1){
    wnd_y++;
  }
  else{
    wnd_y = -8;
  }
  
  if(pattern == inv_a){
    pattern = inv_b;
  }
  else{
    pattern = inv_a;
  }
  
  delay(500);
}

