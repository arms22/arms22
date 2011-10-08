#include <LiquidCrystal.h>
#include <BigFont.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
BigFont bigf;

void setup() {
  lcd.begin(20, 4);
  bigf.attach(&lcd);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available()) {
    delay(100);
    bigf.clear();
    while (Serial.available() > 0) {
      char c = Serial.read();
      bigf.write(c);
      Serial.print(c);
    }
  }
}

