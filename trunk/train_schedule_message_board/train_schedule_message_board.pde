#include <AkiMatrix.h>
#include <Wire.h>
#include <RTC8564.h>
#include <Metro.h>
#include <Debounce.h>
#include "Timeline.h"
#include "Font.h"

// pin 2: SIN1
// pin 3: SIN2
// pin 4: SIN3
// pin 5: CLK
// pin 6: LATCH
// pin 7: STROB
// pin 8: SW1
// pin 9: SW2
// pin10: SW3
// pin11: SW4
// pin12: SW5

// (analog in)pin 4: SCL
// (analog in)pin 5: SDA

AkiMatrix myMatrix = AkiMatrix(2,3,4,5,6,7);
Metro timelineChecker = Metro(5000);

Debounce sw1 = Debounce(20, 8);
Debounce sw2 = Debounce(20, 9);
Debounce sw3 = Debounce(20,10);
Debounce sw4 = Debounce(20,11);
Debounce sw5 = Debounce(20,12);

#define KEY_DOWN LOW
#define KEY_UP   HIGH

uint8_t setting_hour_10;
uint8_t setting_hour_1;
uint8_t setting_minute_10;
uint8_t setting_minute_1;

void drawUnicodeString(uint8_t x,uint8_t y,const uint16_t *str,int len)
{
  int i;
  for(i=0;i<len;i++){
    uint8_t w,h;
    uint8_t bitmap[24];
    uint16_t e = *str++;
    if(Font.getBitmap(e, &w, &h, bitmap)){
      myMatrix.write(x, y, w, h, bitmap);
      x += w;
    }
    else{
      Serial.print(i,DEC);
      Serial.print(" ");
      Serial.print(e,DEC);
      Serial.println(" => font not found");
      x += 6;
    }
  }
}

uint16_t* strcpy_UP(uint16_t *d,const prog_uint16_t *s)
{
  uint16_t *ret = d;
  uint16_t c;
  do {
    c = pgm_read_word(s++);
    *d++ = c;
  } 
  while(c);
  return ret;
}

uint16_t* strcpy_U(uint16_t *d,const prog_char *s)
{
  uint16_t *ret = d;
  char c;
  do {
    c = pgm_read_byte(s++);
    *d++ = c;
  } 
  while(c);
  return ret;
}

int strlen_U(const uint16_t *s)
{
  int ret = 0;
  while(*s++)ret++;
  return ret;
}

void displayTimeline(void)
{
  uint16_t strbuf[16];  

  strbuf[0] = '0' + (Timeline.hours() / 10);
  strbuf[1] = '0' + (Timeline.hours() % 10);
  strbuf[2] = ':';
  strbuf[3] = '0' + (Timeline.minutes() / 10);
  strbuf[4] = '0' + (Timeline.minutes() % 10);

  myMatrix.clear();
  drawUnicodeString(0, 2, strbuf, 5);

  strcpy_UP(strbuf, Timeline.destinationString());
  drawUnicodeString(36, 2, strbuf, strlen_U(strbuf));

  Serial.print("timeline ");
  Serial.print( Timeline.hours(), DEC);
  Serial.print(":");
  Serial.print( Timeline.minutes(), DEC);
  Serial.print(" ");
  Serial.println( Timeline.destination(), DEC);
}

void displaySetting(void)
{
  uint16_t strbuf[16];

  strbuf[0] = '0' + setting_hour_10;
  strbuf[1] = '0' + setting_hour_1;
  strbuf[2] = ':';
  strbuf[3] = '0' + setting_minute_10;
  strbuf[4] = '0' + setting_minute_1;

  myMatrix.clear();
  drawUnicodeString(0, 2, strbuf, 5);

  Serial.print("setting ");
  Serial.print( setting_hour_10, DEC);
  Serial.print( setting_hour_1, DEC);
  Serial.print(":");
  Serial.print( setting_minute_10, DEC);
  Serial.println( setting_minute_1, DEC);
}

uint8_t last_days;

void setup(){
  pinMode( 8, INPUT);
  pinMode( 9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

  digitalWrite( 8, HIGH);
  digitalWrite( 9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);

  Serial.begin(9600);

  Rtc.begin();
  Rtc.available();
  last_days = Rtc.days();
  Timeline.trainAt(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal), Rtc.weekdays());

  displayTimeline();
}

#define RUNNING 0
#define SETTING 1

uint8_t state = RUNNING;
bool need_display = false;

void loop(){

  if(state == RUNNING){
    if(timelineChecker.check()){
      Rtc.available();
      if(Timeline.passed(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal), Rtc.days() != last_days)){
        if(Timeline.nextTrain()){
          ;
        }
        else{
          last_days = Rtc.days();
          Timeline.trainAt(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal), Rtc.weekdays());
        }
        need_display = true;
      }
    }
    if(sw1.update() && sw1.read() == KEY_DOWN){
      Rtc.available();
      setting_hour_10 = Rtc.hours() >> 4;
      setting_hour_1  = Rtc.hours() & 0x0f;
      setting_minute_10 = Rtc.minutes() >> 4;
      setting_minute_1  = Rtc.minutes() & 0x0f;

      state = SETTING;
      need_display = true;
    }
  }
  else{
    if(sw2.update() && sw2.read() == KEY_DOWN){
      setting_hour_10++;
      if(setting_hour_10 > 2){
        setting_hour_10 = 0;
      }
      else if(setting_hour_10 == 2){
        if(setting_hour_1 > 3){
          setting_hour_1 = 3;
        }
      }
      need_display = true;
    }
    if(sw3.update() && sw3.read() == KEY_DOWN){
      setting_hour_1++;
      if(setting_hour_10 < 2){
        if(setting_hour_1 > 9){
          setting_hour_1 = 0;
        }
      }
      else{
        if(setting_hour_1 > 3){
          setting_hour_1 = 0;
        }        
      }
      need_display = true;
    }
    if(sw4.update() && sw4.read() == KEY_DOWN){
      setting_minute_10++;
      if(setting_minute_10 > 5){
        setting_minute_10 = 0;
      }
      need_display = true;
    }
    if(sw5.update() && sw5.read() == KEY_DOWN){
      setting_minute_1++;
      if(setting_minute_1 > 9){
        setting_minute_1 = 0;
      }
      need_display = true;
    }
    if(sw1.update() && sw1.read() == KEY_DOWN){
      uint8_t times[3];
      times[0] = 0;
      times[1] = (setting_minute_10 << 4) | setting_minute_1;
      times[2] = (setting_hour_10 << 4) | setting_hour_1;
      Rtc.sync(times,3);
      Rtc.available();
      state = RUNNING;
      if(Timeline.trainAt(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal))){
        ;
      }
      else{
        Timeline.firstTrain();
      }
      need_display = true;
    }
  }
  if(myMatrix.vsync() && need_display){
    need_display = false;
    if(state == RUNNING){
      displayTimeline();
    }
    else{
      displaySetting();
    }
  }
}









