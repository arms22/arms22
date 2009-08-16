#include <Distance.h>

Distance myDistance = Distance(4);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  myDistance.available();
  Serial.print(myDistance.analogValue());
  Serial.print(", ");
  Serial.print(myDistance.numberOfDistance());
  Serial.println("mm");
  delay(300);
}
