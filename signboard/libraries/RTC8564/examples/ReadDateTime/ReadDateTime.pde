#include <Wire.h>
#include <RTC8564.h>

void setup()
{
  Serial.begin(9600);
  Rtc.begin();
}

void loop()
{
  delay(1000);
  Rtc.available();
  
  Serial.print(0x2000+Rtc.years(),HEX);
  Serial.print("/");
  Serial.print(Rtc.months(),HEX);
  Serial.print("/");
  Serial.print(Rtc.days(),HEX);
  Serial.print(" ");
  Serial.print(Rtc.hours(),HEX);
  Serial.print(":");
  Serial.print(Rtc.minutes(),HEX);
  Serial.print(":");
  Serial.println(Rtc.seconds(),HEX);
}
