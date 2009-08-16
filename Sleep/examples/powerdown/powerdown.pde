#include <Sleep.h>

int count = 0;

void setup(void)
{
  pinMode(2, INPUT);
  Serial.begin(9600);
}

void loop(void)
{
  Serial.print("Awake for ");
  Serial.print(count);
  Serial.println("sec");
  count++;
  delay(1000);
  
  if (Serial.available()) {
    int val = Serial.read();
    if (val == 'S') {
      Serial.println("Serial: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep 
                      // function will provoke a Serial error otherwise!!
      count = 0;
      Sleep::powerDownAndWakeupExternalEvent(0);     // sleep function called here
    }
    if (val == 'A') {
      Serial.println("Hola Caracola"); // classic dummy message
    }
  }
  
  // check if it should go asleep because of time
  if (count >= 10) {
    Serial.println("Timer: Entering Sleep mode");
    delay(100);     // this delay is needed, the sleep 
                    // function will provoke a Serial error otherwise!!
    count = 0;
    Sleep::powerDownAndWakeupExternalEvent(0);     // sleep function called here
  }
}
