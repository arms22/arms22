#include <AkiMatrix.h>

/*
  Hello Matrix
  by arms22 <http://arms22.blog91.fc2.com/>

  Demonstrates the use of the AkiMatrix library
  
  pin 0: sin1 (row)
  pin 1: sin2 (led1)
  pin 2: sin3 (led2)
  pin 3: clk
  pin 4: latch (inverted)
  pin 5: strob
*/
AkiMatrix myMatrix = AkiMatrix(0,1,2,3,4,5);

void setup()
{
  int x,y;
  for(y=0;y<AKI_MATRIX_NUMBER_OF_ROWS;y++){
    for(x=y&1;x<AKI_MATRIX_NUMBER_OF_COLS;x+=2){
      myMatrix.write(x,y,HIGH);
    }
  }
  myMatrix.setOutputEnable(true);
}

void loop()
{
  myMatrix.hsync();
  delay(1);
}
