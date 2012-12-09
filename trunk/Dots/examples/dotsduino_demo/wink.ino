const byte wink_ptn_1[] = {
  B01000010,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
};

const byte wink_ptn_2[] = {
  B00000000,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
};

void wink()
{
  int num = (random() % 2) + 1;
  for(int i=0; i<num; i++){
    myDots.write(2, wink_ptn_2, 5);
    if(delayUntilStateChange(100 + (random() % 50)))return;
    
    myDots.write(2, wink_ptn_1, 5);
    if(delayUntilStateChange(100 + (random() % 50)))return;
  }
  delayUntilStateChange(1000 + (random() % 4000));
}


