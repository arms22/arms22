const byte ufo[] = {
  B00000100,
  B00001100,
  B00011110,
  B00110111,
  B00111110,
  B01111100,
  B01110100,
  B01111110,
  B01111110,
  B01110100,
  B01111100,
  B00111110,
  B00110111,
  B00011110,
  B00001100,
  B00000100,
};

void inverder_ufo(void)
{
  static int img_h = sizeof(ufo) / sizeof(ufo[0]);
  static int wnd_y = -7;
  static int step_y = 1;

  myDots.clear();

  if(wnd_y < 0){
    myDots.write(-wnd_y, &ufo[0], 8 + wnd_y);
  }
  else if(wnd_y < (img_h - 8)){
    myDots.write(0, &ufo[wnd_y], 8);
  }
  else{
    myDots.write(0, &ufo[wnd_y], img_h - wnd_y);
  }

  wnd_y += step_y;
  if((wnd_y > img_h) || (wnd_y < -8)){
    if(delayUntilStateChange( 2000 + (random() % 6000) ))return;  // 2秒〜8秒待って折り返す
    step_y = -step_y;
    wnd_y += step_y;
  }

  delayUntilStateChange(80);
}


