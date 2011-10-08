/*** Wave Animation
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

const byte wave[] = {
  B00110000,
  B00001100,
  B00000010,
  B00000010,
  B00001100,
  B00110000,
  B01000000,
  B01000000,
};

void setup()
{
  myDots.begin();
}

int x = 0;

void loop()
{
  int len = 8 - x;
  myDots.write(x, wave, len);
  myDots.write(0, wave+len, x);
  x++;
  if(x == 8) x = 0;
  delay(80);
}
