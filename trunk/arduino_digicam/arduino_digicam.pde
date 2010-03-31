#include <stdio.h>
#include <FatFs.h>
#include <CameraC328R.h>
#include <NewSoftSerial.h>
#include <Bounce.h>
#include <EEPROM.h>
#include <Sleep.h>

// GPS制御端子
const int gpsRxPin    = 2;
const int gpsTxPin    = 4;
const int gpsPowerPin = 8;

// LED端子
const int ledRedPin   = 5;
const int ledGrnPin   = 6;

// スイッチ端子
const int wakeupPin   = 3;

// バッテリー電圧アナログ端子
const int vBatteryPin = 5;

// カメラ通信
CameraC328R camera;

// GPS通信
NewSoftSerial gps(gpsRxPin, gpsTxPin);

// 写真保存
File file;

// シャッタースイッチ
Bounce button(wakeupPin, 10);

// 関数プロトタイプ
void sleep(unsigned long ms);
void green(uint8_t on_off);
void attention(uint8_t on_off);
void heartbeat(bool force = false);
void takePicture(void);

void setup()
{
  Serial.begin(38400);

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGrnPin, OUTPUT);

  pinMode(wakeupPin, INPUT);
  digitalWrite(wakeupPin, HIGH);

  sleep(11);
  button.update();

  sleep(11);
  button.update();

  // FATファイルシステム初期化
  if( FatFs.initialize() ){
    // 写真保存用フォルダ作成
    char dir[] = "photos";
    FatFs.createDirectory(dir);
    // 写真保存用フォルダへ移動
    if( !FatFs.changeDirectory(dir) ){
      goto init_error;
    }
    green(HIGH);
  }
  else{
init_error:
    attention(HIGH);
    while(1);
  }
}

unsigned long last_heartbeat = 0;
unsigned long last_takepict  = 0;
unsigned long low_duration   = 0;
bool interval_mode           = false;

void loop()
{
  // ボタン離されたら
  if( button.update() && (button.read() == HIGH) ){
    // 8秒以上長押ししていたら
    if(low_duration > 8000 ){
      ;
    }
    // 3秒以上長押ししていたら
    else if( low_duration > 3000 ){
      // インターバル撮影モード
      interval_mode = true;
      last_takepict = millis();
      green(HIGH);
    }
    else {
      // インターバル撮影モードでの単押しの場合
      if( interval_mode ){
        // インターバル撮影モードオフ
        interval_mode = false;
        heartbeat();
      }
      else{
        // 単押しは通常撮影
        takePicture();
      }
    }
  }

  // ボタンが長押し押されている時間を保存しておく
  if( button.read() == LOW ){
    low_duration = button.duration();
  }

  // インターバル撮影モードなら最後の撮影から3秒経過していたら撮影する
  if( interval_mode && (millis() - last_takepict > 3000) ){
    takePicture();
    last_takepict = millis();
  }

  // ハートビート
  if(!interval_mode){
    heartbeat();
  }

  sleep(10);
}

void sleep(unsigned long ms){
  unsigned long start = millis();
  while (millis() - start <= ms){
    Sleep.idle();
  }
}

void green(uint8_t on_off){
  if(on_off){
    analogWrite(ledGrnPin, 128);
  }
  else{
    digitalWrite(ledGrnPin, LOW);
  }
}

void attention(uint8_t on_off){
  if(on_off){
    analogWrite(ledRedPin, 128);
  }
  else{
    digitalWrite(ledRedPin, LOW);
  }
}

void heartbeat(bool force){
  if( force || (millis() - last_heartbeat > 3000) ){
    green(HIGH);
    sleep(25);
    green(LOW);
    last_heartbeat = millis();
  }
}

void getJPEGPicture_callback( uint16_t pictureSize, uint16_t packageSize, uint16_t packageCount, byte* package )
{
  green(HIGH);
  file.write(package, packageSize);
  green(LOW);
}

int sequentialPhotoNumber(void)
{
  int num;
  num = EEPROM.read(0);
  num |= EEPROM.read(1) << 8;
  num++;
  if( num > 9999 ){
    num = 0;
  }
  EEPROM.write(0,num & 0xff);
  EEPROM.write(1,num >> 8);
  return num;
}

void takePicture(void)
{
  uint16_t pictureSize = 0;
  char buf[12];

  green(HIGH);
  attention(LOW);

  if( !camera.sync() ){
    goto camera_error;
  }

  if( !camera.initial( CameraC328R::CT_JPEG, CameraC328R::PR_160x120, CameraC328R::JR_640x480 ) ){
    goto camera_error;
  }

  if( !camera.setPackageSize( 64 ) ){
    goto camera_error;
  }

  if( !camera.setLightFrequency( CameraC328R::FT_50Hz ) ){
    goto camera_error;
  }

  attention(HIGH);
  if( !camera.snapshot( CameraC328R::ST_COMPRESSED, 0 ) ){
    goto camera_error;
  }
  attention(LOW);

  snprintf(buf, sizeof(buf), "img%04d.jpg", sequentialPhotoNumber());

  if( FatFs.fileExists(buf) ){
    FatFs.deleteFile(buf);
  }

  if( !FatFs.createFile(buf) ){
    goto camera_error;
  }

  if( !file.open(buf) ){
    goto camera_error;
  }

  if( !camera.getJPEGPictureSize( CameraC328R::PT_SNAPSHOT, PROCESS_DELAY, pictureSize) ){
    goto camera_error;
  }

  file.resize(pictureSize);

  if( !camera.getJPEGPictureData( &getJPEGPicture_callback) ){
    goto camera_error;
  }

  file.close();

  green(LOW);
  return;

  {
camera_error:
    green(LOW);
    attention(HIGH);
    sleep(5000);
    attention(LOW);
  }
}




