#include <EthernetDHCP.h>
#include <Ethernet.h>
#include <TwitPic.h>
#include <CameraC328R.h>
#include <NewSoftSerial.h>
#include <Bounce.h>

#define DOOR_PIN 4
#define LED_PIN 5

Bounce door(DOOR_PIN, 10);

#define STANDBY       0
#define TAKE_PICTURE  1

int state;

#define INITIAL_DELAY (2*1000)
#define INTERVAL      (30*1000)

unsigned long take_picture_time;

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your TwitPic API Key(key)
const prog_char twitpic_api_key[] PROGMEM     = "Your TwitPic API Key";

// Your Twitter Consumer key(consumer_token)/Consumer Secret(consumer_secret)
const prog_char consumer_key[] PROGMEM        = "Your Consumer key";
const prog_char consumer_secret[] PROGMEM     = "Your Consumer Secret";

// Your Twitter Access Token (oauth_token)/Access Token Secret (oauth_token_secret)
const prog_char access_token[] PROGMEM        = "Your Access Token";
const prog_char access_token_secret[] PROGMEM = "Your Access Token Secret";

TwitPic twitpic;

#define USB_BAUD 115200
#define CAMERA_BAUD 14400

NewSoftSerial mySerial(2, 3);
CameraC328R camera(&mySerial);

void setup()
{
  delay(1000);

  Serial.begin(USB_BAUD);
  mySerial.begin(CAMERA_BAUD);

  pinMode(DOOR_PIN, INPUT);
  digitalWrite(DOOR_PIN, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  state = STANDBY;
  Serial.println("standby...");

  ethernet_begin_with_dhcp();
}

#define STANDBY_BLINK_INTERVAL (10*1000)
unsigned long standby_blink_timing = 0;

void loop()
{
  door.update();

  if(STANDBY == state){
    if(millis() > standby_blink_timing){
      led_on();
      delay(50);
      led_off();
      standby_blink_timing = millis() + STANDBY_BLINK_INTERVAL;
    }
    if(is_door_open()){
      state = TAKE_PICTURE;
      take_picture_time = millis() + INITIAL_DELAY;
      led_on();
    }
  }
  else if(TAKE_PICTURE == state){
    if(is_door_open()){
      if(millis() > take_picture_time){
        take_picture_time = millis() + INTERVAL;
        takePicture();
      }
      else{
        camera.sync();
      }
    }
    else{
      state = STANDBY;
      led_off();
    }
  }

  EthernetDHCP.maintain();
}

void ethernet_begin_with_dhcp(void)
{
  EthernetDHCP.begin(mac,true);

  do {
    DhcpState state = EthernetDHCP.poll();
    if(state == DhcpStateLeased){
      break;
    }
    led_on();
    delay(50);
    led_off();
    delay(450);
  }
  while(1);

  print_ip_addr("My IP ", EthernetDHCP.ipAddress());
  print_ip_addr("Gateway IP ", EthernetDHCP.gatewayIpAddress());
  print_ip_addr("DNS IP ", EthernetDHCP.dnsIpAddress());

  Serial.println();

  led_off();
}

void print_ip_addr(const char *msg,const uint8_t* ipAddr)
{
  Serial.print(msg);
  Serial.print(ipAddr[0],DEC);
  Serial.print(".");
  Serial.print(ipAddr[1],DEC);
  Serial.print(".");
  Serial.print(ipAddr[2],DEC);
  Serial.print(".");
  Serial.print(ipAddr[3],DEC);
  Serial.println();
}

void led_on(void)
{
  digitalWrite(LED_PIN, HIGH);
}

void led_off(void)
{
  digitalWrite(LED_PIN, LOW);
}

void led_toggle(void)
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

bool is_door_open(void)
{
  return (door.read() == LOW);
}



uint16_t pictureSize = 0;
Client *targetClient;

void getJPEGPicture_callback( uint16_t pictureSize, uint16_t packageSize, uint16_t packageCount, byte* package )
{
  targetClient->write(package,packageSize);
  led_toggle();
}

void c328rImageTransfer(Client *client)
{
  targetClient = client;
  camera.getJPEGPictureData( &getJPEGPicture_callback );
}

void takePicture(void)
{
  int ret;

  Serial.print( "sync..." );
  if( !camera.sync() ){
    Serial.println( "failed" );
    goto camera_error;
  }

  Serial.print( "\ninitial..." );
  if( !camera.initial( CameraC328R::CT_JPEG, CameraC328R::PR_160x120, CameraC328R::JR_640x480 ) ){
    Serial.println( "failed" );
    goto camera_error;
  }

  if( !camera.setPackageSize( 64 ) ){
    goto camera_error;
  }

  if( !camera.setLightFrequency( CameraC328R::FT_50Hz ) ){
    goto camera_error;
  }

  if( !camera.snapshot( CameraC328R::ST_COMPRESSED, 0 ) ){
    goto camera_error;
  }

  pictureSize = 0;
  if( !camera.getJPEGPictureSize( CameraC328R::PT_SNAPSHOT, PROCESS_DELAY, pictureSize) ){
    goto camera_error;
  }

  Serial.print( "\nbegin upload..." );
  //ret = twitpic.upload(pictureSize, &c328rImageTransfer);
  ret = twitpic.uploadAndPost("Hello Maker! I am Refrigerator! #tweetingfridge", pictureSize, &c328rImageTransfer);
  if( ret < 0 ){
    Serial.print("post failed");
    Serial.println(ret);
    goto camera_error;
  }

  //  camera.powerOff();

  Serial.println("\ndone.");
  led_on();
  return;

camera_error:
  ;
}


