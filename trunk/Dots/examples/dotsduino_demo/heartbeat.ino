
const byte heart_big_ptn[] = {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
};

const byte heart_small_ptn[] = {
  B00000000,
  B00000000,
  B00100100,
  B01111110,
  B01111110,
  B00111100,
  B00011000,
  B00000000,
};

void heartbeat()
{
  myDots.write(0, heart_big_ptn, 8);
  if(delayUntilStateChange(750))return;

  myDots.write(0, heart_small_ptn, 8);
  if(delayUntilStateChange(250))return;
}

