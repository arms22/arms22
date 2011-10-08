/*** Hello Matrix
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

void setup()
{
  myDots.begin();
  myDots.write(1, 5, HIGH);
  myDots.write(2, 2, HIGH);
  myDots.write(2, 6, HIGH);
  myDots.write(3, 6, HIGH);
  myDots.write(4, 6, HIGH);
  myDots.write(5, 2, HIGH);
  myDots.write(5, 6, HIGH);
  myDots.write(6, 5, HIGH);
}

void loop()
{
}
