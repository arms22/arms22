const byte wave_ptn[] = {
  B00110000,
  B00001100,
  B00000010,
  B00000010,
  B00001100,
  B00110000,
  B01000000,
  B01000000,
};

void wave()
{
  static int x = 0;
  int len = 8 - x;
  myDots.write(x, wave_ptn, len);
  myDots.write(0, wave_ptn+len, x);
  x++;
  if(x == 8) x = 0;
  if(delayUntilStateChange(80))return;
}

