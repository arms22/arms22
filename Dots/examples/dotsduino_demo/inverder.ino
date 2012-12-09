const byte inv_a[] = {
  B11111100,
  B10001110,
  B00011011,
  B00101110,
  B00101110,
  B00011011,
  B10001110,
  B11111100,
};

const byte inv_b[] = {
  B11111000,
  B10111100,
  B00010101,
  B01111110,
  B01111110,
  B00010101,
  B10111100,
  B11111000,
};

void inverder(void)
{
  static int y = 0;
  myDots.clear();
  if(y < 8){
    myDots.write(0, &inv_a[8-y], y);
  }
  else if(y == 8){
    myDots.write(0, inv_a, 8);
    if(delayUntilStateChange(150))return;
    myDots.write(0, inv_b, 8);
    if(delayUntilStateChange(150))return;
    myDots.write(0, inv_a, 8);
    if(delayUntilStateChange(150))return;
    myDots.write(0, inv_b, 8);    
    if(delayUntilStateChange(150))return;
    myDots.write(0, inv_a, 8);
  }
  else if(y < 16){
    myDots.write(y-8, inv_a, 16-y);
  }
  else{
    y = 0;
  }
  y++;
  if(delayUntilStateChange(100))return;
}


