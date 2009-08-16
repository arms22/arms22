#include <ColorSensor.h>

uint8_t _pinLED = 5;
uint8_t _pinSW1 = 6;
uint8_t _pinSW2 = 7;

ColorSensor myCS = ColorSensor(2,3,4);

void setup(){
  pinMode(_pinSW1, INPUT);
  digitalWrite(_pinSW1, HIGH);

  pinMode(_pinSW2, INPUT);
  digitalWrite(_pinSW2, HIGH);

  pinMode(_pinLED, OUTPUT);  
  digitalWrite(_pinLED, LOW);
  
  myCS.colorReference(ColorSensor::COLOR_REF_MANUAL);

  Serial.begin(9600);
}

void loop(){
  // LED on
  digitalWrite(_pinLED, HIGH);

  if(!digitalRead(_pinSW1)){
    // adjust white color
    myCS.adjustWhite();
  }else if(!digitalRead(_pinSW2)){
    // adjust black color
    myCS.adjustBlack();
  }else{
    // measure color
    myCS.measureColor();
     
    Serial.print("Red = ");
    Serial.print(myCS.redValue());
    
    Serial.print(" Green = ");
    Serial.print(myCS.greenValue());
    
    Serial.print(" Blue = ");
    Serial.println(myCS.blueValue());
  }
  
  // LED off
  digitalWrite(_pinLED, LOW);
  
  delay(1000);
}
