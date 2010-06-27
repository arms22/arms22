#include <Ethernet.h>
#include <TwitPic.h>
#include "SampleImage.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 10, 0, 0, 177 };

TwitPic twitpic;

uint32_t sampleImageTransfer(Client *client)
{
  if(client){
    for(uint32_t i=0; i<sizeof(sampleImage); i++){
      client->print(pgm_read_byte(sampleImage+i));
    }
  }
  return sizeof(sampleImage);
}

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

  delay(3000);

  int ret = twitpic.uploadAndPost("Sample Image Post from Arduino", &sampleImageTransfer);
  if(ret < 0){
    Serial.print("post failed : err code = ");
    Serial.println(ret);
  }
  else{
    Serial.println("OK.");
  }
}

void loop()
{
}

