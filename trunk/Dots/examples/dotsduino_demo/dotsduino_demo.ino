/*** Dotsduino Demo
 * by arms22 <http://arms22.blog91.fc2.com/>
 * 
 * Demonstrates the use of the Dots library
 *
 */

#include <Dots.h>
#include <Bounce.h>

const int BUTTON = 18;  // Analog 4

Dots myDots = Dots();
Bounce bouncer = Bounce(BUTTON, 10);
int state = 0;

boolean delayUntilStateChange(unsigned long ms)
{
  boolean ret = false;
  unsigned long t = millis();
  do {
    if(bouncer.update()){
      if(bouncer.read() == LOW){
        if(state < 6){
          state++;
        }
        else{
          state = 0;
        }
        myDots.clear();
        ret = true;
        break;
      }
    }
  }
  while(millis() - t < ms);
  return ret;
}

void setup()
{
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  myDots.begin();
}

void loop()
{
  switch(state){
  case 0:
    heartbeat();
    break;
  case 1:
    wave();
    break;
  case 2:
    wink();
    break;
  case 3:
    inverder();
    break;
  case 4:
    inverder_kani();
    break;
  case 5:
    inverder_ufo();
    break;
  case 6:
    make_anything();
    break;
  }
}








