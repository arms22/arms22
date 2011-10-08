/*** Flip Animation
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

const byte one[] = {
  B01000010,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
};

const byte two[] = {
  B00000000,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
};

void setup()
{
  myDots.begin();
}

void loop()
{
  int num = (random() % 2) + 1;
  for(int i=0; i<num; i++){
    myDots.write(2, two, 5);
    delay(100 + (random() % 50));
    
    myDots.write(2, one, 5);
    delay(100 + (random() % 50));
  }
  delay(1000 + (random() % 4000));
}


