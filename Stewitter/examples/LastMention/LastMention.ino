#include <SPI.h>
#include <Ethernet.h>
#include <Stewitter.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
Stewitter twitter("YourToken");

void setup()
{
  delay(1000);

  Ethernet.begin(mac);

  Serial.begin(9600);
  Serial.println("connecting ...");
  if (twitter.lastMention()) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println(twitter.response());
    } 
    else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } 
  else {
    Serial.println("connection failed.");
  }
}

void loop()
{
}

