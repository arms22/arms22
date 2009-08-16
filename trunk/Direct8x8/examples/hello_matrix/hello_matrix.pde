#include <Direct8x8.h>

/*
  Hello Matrix
  by arms22 <http://arms22.blog91.fc2.com/>

  Demonstrates the use of the Direct8x8 library
  
  pin 0- 7: row0-row7
  pin 8-12,14-16: row8-row12, row13-row15
*/
Direct8x8 myMatrix = Direct8x8( 0, 1, 2, 3, 4, 5, 6, 7,
                                8, 9,10,11,12,14,15,16);

void setup()
{
  // turn some pixels on
  myMatrix.write(1, 5, HIGH);
  myMatrix.write(2, 2, HIGH);
  myMatrix.write(2, 6, HIGH);
  myMatrix.write(3, 6, HIGH);
  myMatrix.write(4, 6, HIGH);
  myMatrix.write(5, 2, HIGH);
  myMatrix.write(5, 6, HIGH);
  myMatrix.write(6, 5, HIGH);
}

void loop()
{
  // display matrix
  myMatrix.vsync();
}
