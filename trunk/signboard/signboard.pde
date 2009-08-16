#include <AkiMatrix.h>
#include <Wire.h>
#include <RTC8564.h>
#include <Timeline.h>
#include <Metro.h>
#include <Debounce.h>
//#include "jr_saikyo_line_timeline.h"
//#include "jr_saikyo_line_images.h"
#include "keiou_sagamihara_line_timeline.h"
#include "keiou_sagamihara_line_images.h"
#include "number_images.h"

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

//#define MY_TIMELINE        jr_saikyo_line_shibuya_north_business_day
//#define MY_TIMELINE_IMAGES jr_saikyo_line_images

#define MY_TIMELINE        keiou_sagamihara_tama_center_for_tyoufu_holiday
#define MY_TIMELINE_IMAGES keiou_sagamihara_line_images
//#define MY_TIMELINE        keiou_sagamihara_tama_center_for_hashimoto_holiday
//#define MY_TIMELINE_IMAGES keiou_sagamihara_line_images

AkiMatrix myMatrix = AkiMatrix(2,3,4,5,6,7);
Timeline myTimeline = Timeline(MY_TIMELINE);
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

void displayTimeline(void)
{
  myMatrix.clear();

  MyImage numimg = numberImages('0' + (myTimeline.hours() / 10));
  myMatrix.bitBlt_P( 0, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + (myTimeline.hours() % 10));
  myMatrix.bitBlt_P( 8, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages(':');
  myMatrix.bitBlt_P(16, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + (myTimeline.minutes() / 10));
  myMatrix.bitBlt_P(24, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + (myTimeline.minutes() % 10));
  myMatrix.bitBlt_P(32, 0, numimg.data, numimg.width, numimg.height);

  MyImage dstimg = MY_TIMELINE_IMAGES[myTimeline.destination()];
  myMatrix.bitBlt_P(48, 0, dstimg.data, dstimg.width, dstimg.height);

  Serial.print( myTimeline.hours(), DEC);
  Serial.print(":");
  Serial.print( myTimeline.minutes(), DEC);
  Serial.print(" ");
  Serial.println( myTimeline.destination(), DEC);
}

void displaySetting(void)
{
  myMatrix.clear();

  MyImage numimg = numberImages('0' + setting_hour_10);
  myMatrix.bitBlt_P( 0, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + setting_hour_1);
  myMatrix.bitBlt_P( 8, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages(':');
  myMatrix.bitBlt_P(16, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + setting_minute_10);
  myMatrix.bitBlt_P(24, 0, numimg.data, numimg.width, numimg.height);

  numimg = numberImages('0' + setting_minute_1);
  myMatrix.bitBlt_P(32, 0, numimg.data, numimg.width, numimg.height);

  Serial.print( setting_hour_10, DEC);
  Serial.print( setting_hour_1, DEC);
  Serial.print(":");
  Serial.print( setting_minute_10, DEC);
  Serial.println( setting_minute_1, DEC);
}

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
  
  myTimeline.trainAt(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal));
  
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
      if((Rtc.hours(RTC8564::Decimal) == myTimeline.hours()) &&
        (Rtc.minutes(RTC8564::Decimal) == myTimeline.minutes())){
        if(myTimeline.nextTrain()){
          ;
        }
        else{
          myTimeline.firstTrain();
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
      if(myTimeline.trainAt(Rtc.hours(RTC8564::Decimal), Rtc.minutes(RTC8564::Decimal))){
        ;
      }
      else{
        myTimeline.firstTrain();
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
