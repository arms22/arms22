#include <AkiMatrix.h>
#include <Wire.h>
#include <RTC8564.h>
#include <Metro.h>
#include <Debounce.h>
#include <TimerOne.h>
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
Metro timeChecker = Metro(15000);

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
uint8_t setting_weekday;
uint8_t setting_cursor;

bool need_display = false;
bool train_available = false;

#define RUNNING 0
#define SETTING 1

uint8_t state = RUNNING;

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
      //      Serial.print(i,DEC);
      //      Serial.print(" ");
      //      Serial.print(e,DEC);
      //      Serial.println(" => font not found");
      x += 4;
    }
  }
}

//uint16_t utf8_to_ucs2(const char *str)
//{
//  uint16_t ucs2;
//  char c = *str++;
//  if((c <= 0x7F)){
//    ucs2 = c;
//  }
//  else if((c >= 0xC0) && (c <= 0xDF)){
//    ucs2 = (c & 0x1F) << 6;
//    ucs2 += (*str++ & 0x3F);
//  }
//  else if((c >= 0xE0) && (c <= 0xEF)){
//    ucs2 = (c & 0x0F) << 12;
//    ucs2 += (*str++ & 0x3F) << 6;
//    ucs2 += (*str++ & 0x3F);
//  }
//}

uint16_t* u_strcpy_P(uint16_t *d,const prog_uint16_t *s)
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

uint16_t* u_strcpy_P(uint16_t *d,const prog_char *s)
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

int u_strlen(const uint16_t *s)
{
  int ret = 0;
  while(*s++)ret++;
  return ret;
}

void displayTimeline(void)
{
  uint16_t strbuf[16];  

  if(train_available){
    strbuf[0] = '0' + (Timeline.hours() / 10);
    strbuf[1] = '0' + (Timeline.hours() % 10);
    strbuf[2] = ':';
    strbuf[3] = '0' + (Timeline.minutes() / 10);
    strbuf[4] = '0' + (Timeline.minutes() % 10);
    u_strcpy_P(&strbuf[5], Timeline.destinationString());

    myMatrix.clear();
    drawUnicodeString(0, 2, strbuf, u_strlen(strbuf));

    //    Serial.print("timeline ");
    //    Serial.print( Timeline.hours(), DEC);
    //    Serial.print(":");
    //    Serial.print( Timeline.minutes(), DEC);
    //    Serial.print(" ");
    //    Serial.println( Timeline.destination(), DEC);
  }
  else{
    myMatrix.clear();
    drawUnicodeString(0, 2, u_strcpy_P(strbuf,PSTR("Out of Service")), u_strlen(strbuf));

    //    Serial.println( "Out of Service");
  }
}

void drawCursor(uint8_t x,uint8_t y,uint8_t w,uint8_t h)
{
  int i,j;
  for(i=0;i<h;i++)
    for(j=0;j<w;j++)
      myMatrix.write(x+j, y+i, 1);
}

void displaySetting(void)
{
  uint16_t strbuf[16];

  strbuf[0] = '0' + setting_hour_10;
  strbuf[1] = '0' + setting_hour_1;
  strbuf[2] = ':';
  strbuf[3] = '0' + setting_minute_10;
  strbuf[4] = '0' + setting_minute_1;
  switch(setting_weekday){
  case 0: 
    u_strcpy_P(&strbuf[5],PSTR("Sun")); 
    break;
  case 1: 
    u_strcpy_P(&strbuf[5],PSTR("Mon")); 
    break;
  case 2: 
    u_strcpy_P(&strbuf[5],PSTR("Thu")); 
    break;
  case 3: 
    u_strcpy_P(&strbuf[5],PSTR("Wed")); 
    break;
  case 4: 
    u_strcpy_P(&strbuf[5],PSTR("Thu")); 
    break;
  case 5: 
    u_strcpy_P(&strbuf[5],PSTR("Fri")); 
    break;
  case 6: 
    u_strcpy_P(&strbuf[5],PSTR("Sat")); 
    break;
  }

  myMatrix.clear();
  drawUnicodeString(0, 2, strbuf, u_strlen(strbuf));
  switch(setting_cursor){
  case 0: 
    drawCursor( 0,15, 7,1); 
    break;
  case 1: 
    drawCursor( 7,15, 7,1); 
    break;
  case 2: 
    drawCursor(21,15, 7,1); 
    break;
  case 3: 
    drawCursor(28,15, 7,1); 
    break;
  case 4: 
    drawCursor(35,15,21,1); 
    break;
  }

  //  Serial.print("setting ");
  //  Serial.print( setting_hour_10, DEC);
  //  Serial.print( setting_hour_1, DEC);
  //  Serial.print(":");
  //  Serial.print( setting_minute_10, DEC);
  //  Serial.print( setting_minute_1, DEC);
  //  Serial.print(" ");
  //  Serial.println( setting_weekday, DEC);
}

void refreshMatrix(void){
  myMatrix.hsync();
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

  Timer1.initialize(1000);
  Timer1.attachInterrupt(refreshMatrix);
  Timer1.start();

  //  Serial.begin(9600);

  Rtc.begin();
  Rtc.available();
  train_available = Timeline.trainAt(Rtc.hours(RTC8564::Decimal),
  Rtc.minutes(RTC8564::Decimal),
  Rtc.weekdays());

  displayTimeline();
}

void loop(){

  if(state == RUNNING){
    if(timeChecker.check()){
      Rtc.available();
      if(train_available){
        if(Timeline.passed(Rtc.hours(RTC8564::Decimal),
        Rtc.minutes(RTC8564::Decimal))){
          train_available = Timeline.nextTrain();
          need_display = true;
        }
      }
      else{
        if(Timeline.trainAt(Rtc.hours(RTC8564::Decimal),
        Rtc.minutes(RTC8564::Decimal),
        Rtc.weekdays())){
          train_available = need_display = true;
        }
      }
    }
    if(sw1.update() && sw1.read() == KEY_DOWN){
      Rtc.available();
      setting_hour_10	= Rtc.hours() >> 4;
      setting_hour_1	= Rtc.hours() & 0x0f;
      setting_minute_10 = Rtc.minutes() >> 4;
      setting_minute_1  = Rtc.minutes() & 0x0f;
      setting_weekday	= Rtc.weekdays();
      setting_cursor    = 0;
      state             = SETTING;
      need_display      = true;
    }
  }
  else{
#define VAL_UP(val,max) (val < max ? val+1 : 0)
#define VAL_DW(val,max) (val > 0 ? val-1 : max)

    if(sw2.update() && (sw2.read() == KEY_DOWN)){
      setting_cursor = VAL_UP(setting_cursor,4);
      need_display = true;
    }
    else if(sw3.update() && (sw3.read() == KEY_DOWN)){
      setting_cursor = VAL_DW(setting_cursor,4);
      need_display = true;
    }
    else if(sw4.update() && (sw4.read() == KEY_DOWN)){
      switch(setting_cursor){
      case 0: 
        setting_hour_10	= VAL_UP(setting_hour_10, 2);
        if((setting_hour_10 >= 2) && (setting_hour_1 > 3)) setting_hour_1 = 3;
        break;
      case 1:
        if(setting_hour_10 >= 2)
          setting_hour_1 = VAL_UP(setting_hour_1, 3);
        else
          setting_hour_1 = VAL_UP(setting_hour_1, 9);
        break;
      case 2: 
        setting_minute_10 = VAL_UP(setting_minute_10, 5);			  
        break;
      case 3: 
        setting_minute_1 = VAL_UP(setting_minute_1, 9);			  
        break;
      case 4: 
        setting_weekday	= VAL_UP(setting_weekday, 6);			  
        break;
      }
      need_display = true;
    }
    else if(sw5.update() && (sw5.read() == KEY_DOWN)){
      switch(setting_cursor){
      case 0: 
        setting_hour_10	= VAL_DW(setting_hour_10, 2);			  
        if((setting_hour_10 >= 2) && (setting_hour_1 > 3)) setting_hour_1 = 3;
        break;
      case 1:
        if(setting_hour_10 >= 2)
          setting_hour_1 = VAL_DW(setting_hour_1, 3);
        else
          setting_hour_1 = VAL_DW(setting_hour_1, 9);
        break;
      case 2: 
        setting_minute_10 = VAL_DW(setting_minute_10, 5);			  
        break;
      case 3: 
        setting_minute_1 = VAL_DW(setting_minute_1, 9);			  
        break;
      case 4: 
        setting_weekday	= VAL_DW(setting_weekday, 6);			  
        break;
      }
      need_display = true;
    }
    if(sw1.update() && (sw1.read() == KEY_DOWN)){
      uint8_t times[5];
      times[0] = 0;
      times[1] = (setting_minute_10 << 4) | setting_minute_1;
      times[2] = (setting_hour_10 << 4) | setting_hour_1;
      times[3] = Rtc.days();
      times[4] = setting_weekday;
      Rtc.sync(times,sizeof(times));
      Rtc.available();
      train_available = Timeline.trainAt(Rtc.hours(RTC8564::Decimal),
      Rtc.minutes(RTC8564::Decimal),
      Rtc.weekdays());
      state = RUNNING;
      need_display = true;
    }
  }
  if(need_display){
    need_display = false;
    if(state == RUNNING){
      displayTimeline();
    }
    else{
      displaySetting();
    }
  }
}





