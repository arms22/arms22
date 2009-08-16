#include <ColorSensor.h>

uint8_t _pinLED = 5;

ColorSensor myCS = ColorSensor(4,3,2);

void setup(){
  pinMode(_pinLED, OUTPUT);  
  digitalWrite(_pinLED, LOW);
  Serial.begin(9600);
}

void loop(){
  // LED on
  digitalWrite(_pinLED, LOW);
  
  // measure color
  myCS.measureColor();
  
  // LED off
  digitalWrite(_pinLED, HIGH);
  
  Serial.print("Red = ");
  Serial.print(myCS.redValue());
  
  Serial.print(" Green = ");
  Serial.print(myCS.greenValue());
  
  Serial.print(" Blue = ");
  Serial.println(myCS.blueValue());
  
  delay(1000);
}
