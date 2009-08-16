#include <Distance.h>

/*
Sleep commands
 
 for MacOSX
 $ osascript -e 'tell app "Finder" to sleep'
 */

Distance rightDistance = Distance(4);
Distance leftDistance = Distance(3);

bool autoMeasuringEnable = false;

bool remoteWakeupEnabled = false;
#define INITIAL_INTERVAL_VALUE 1000
short remoteWakeupRepetitionInterval;
long lastWakeupSignal = 0;
int numberOfDistanceForResumeFromSuspend = 700; // millimeter
#define WAKEUP_PIN 2 // 20ms active low pulse

int ledPwmDuty = 64;
int ledPwmStep = 8;
#define LED_PIN 6

void setup()
{
  pinMode(WAKEUP_PIN, OUTPUT);
  digitalWrite(WAKEUP_PIN, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
}

void print_distance(void)
{
  Serial.print(leftDistance.numberOfDistance());
  Serial.print(" ");
  Serial.println(rightDistance.numberOfDistance());  
}

void loop()
{
  rightDistance.available();
  leftDistance.available();

  if (Serial.available() > 0) {
    int c = Serial.read();
    switch(c){
    case '*':
      print_distance();
      break;
    case 'a':
      autoMeasuringEnable = !autoMeasuringEnable;
      break;
    case 'e':
      remoteWakeupEnabled = true;
      lastWakeupSignal = 0;
      ledPwmDuty = 0;
      Serial.println("Remote Wakeup Enabled.");
      break;
    case 'd':
      remoteWakeupEnabled = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("Remote Wakeup Disabled.");
      break;
    case 'w':
      char numbers[32];
      int len;
      for (len = 0; len < (sizeof(numbers) - 1); len++) {
        if (Serial.available() > 0){
          numbers[len] = Serial.read();
        }
        else{
          break;
        }
      }
      if (len) {
        numbers[len] = 0;
        numberOfDistanceForResumeFromSuspend = constrain(atoi(numbers),0,1000);
      }
      Serial.println(numberOfDistanceForResumeFromSuspend);
      break;
    }
  }
  else{

    if (remoteWakeupEnabled) {
      if ((rightDistance.numberOfDistance() < numberOfDistanceForResumeFromSuspend) ||
          (leftDistance.numberOfDistance() < numberOfDistanceForResumeFromSuspend) ){
        if (lastWakeupSignal) {
          if ((millis() - lastWakeupSignal) > remoteWakeupRepetitionInterval){

            digitalWrite(WAKEUP_PIN, LOW);
            delay(100);
            digitalWrite(WAKEUP_PIN, HIGH);

            Serial.println("HEY! WAKE UP!");
            lastWakeupSignal = millis();
          }
        }
        else{
          lastWakeupSignal = millis(); // detect
          remoteWakeupRepetitionInterval = INITIAL_INTERVAL_VALUE;
        }
      }
      else{
        lastWakeupSignal = 0; // clear detect
      }

      if(ledPwmDuty > 255){
        analogWrite(LED_PIN, 255);
      }
      else{
        analogWrite(LED_PIN, ledPwmDuty);
      }

      if(ledPwmDuty > (256+64)){
        ledPwmStep = -8;
      }
      else if(ledPwmDuty < 32){
        ledPwmStep = 8;
      }

      ledPwmDuty += ledPwmStep;
    }

    if (autoMeasuringEnable) {
      print_distance();
    }

    delay(50);
  }
}
