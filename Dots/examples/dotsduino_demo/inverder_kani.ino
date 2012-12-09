const byte inv_kani_a[] = {
  B00001110,
  B00011000,
  B10111110,
  B01101101,
  B00111101,
  B00111100,
  B00111101,
  B01101101,
  B10111110,
  B00011000,
  B00001110,
};

const byte inv_kani_b[] = {
  B01111000,
  B00011101,
  B10111110,
  B01101100,
  B00111100,
  B00111100,
  B00111100,
  B01101100,
  B10111110,
  B00011101,
  B01111000,
};

void inverder_kani(void)
{
  static int img_h = 11;
  static int wnd_y = -8;
  static const byte *pattern = inv_kani_a;

  myDots.clear();

  if(wnd_y < 0){
    myDots.write(-wnd_y, &pattern[0], 8 + wnd_y);
  }
  else if(wnd_y < (img_h - 8)){
    myDots.write(0, &pattern[wnd_y], 8);
  }
  else{
    myDots.write(0, &pattern[wnd_y], img_h - wnd_y);
  }

  if(wnd_y < img_h-1){
    wnd_y++;
  }
  else{
    wnd_y = -8;
  }

  if(pattern == inv_kani_a){
    pattern = inv_kani_b;
  }
  else{
    pattern = inv_kani_a;
  }

  delayUntilStateChange(500);
}




