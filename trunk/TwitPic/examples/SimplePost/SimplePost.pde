#include <Ethernet.h>
#include <TwitPic.h>
#include "SampleImage.h"

// Your TwitPic API Key(key)
const prog_char twitpic_api_key[] PROGMEM     = "Your TwitPic API Key";

// Your Twitter Consumer key(consumer_token)/Consumer Secret(consumer_secret)
const prog_char consumer_key[] PROGMEM        = "Your Consumer key";
const prog_char consumer_secret[] PROGMEM     = "Your Consumer Secret";

// Your Twitter Access Token (oauth_token)/Access Token Secret (oauth_token_secret)
const prog_char access_token[] PROGMEM        = "Your Access Token";
const prog_char access_token_secret[] PROGMEM = "Your Access Token Secret";

TwitPic twitpic;

void sampleImageTransfer(Client *client)
{
  for(uint32_t i=0; i<sizeof(sampleImage); i++){
    client->print(pgm_read_byte(sampleImage+i));
  }
}

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  10, 0, 0, 177 };

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(57600);

  delay(3000);

  int ret = twitpic.uploadAndPost("Sample Image Post from Arduino", sizeof(sampleImage), &sampleImageTransfer);

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

