#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>
#include <BigFont.h>
#include <Time.h>
#include "settings.h"
#include "watt_samples.h"

// ピン定義
const int arefPin = 0;
const int ct1Pin = 1;
const int vt1Pin = 2;
const int ct2Pin = 3;
const int vt2Pin = 4;

// キャラクタLCDクラス（RS=2, R/W=3, E=4, D4=5, D5=6, D4=7, D3=8)
LiquidCrystal lcd(2,3,4,5,6,7,8);

// ビッグフォントクラス
BigFont bigf;

// TCPクライアント
EthernetClient client;

// ホストに接続した最後の時間
unsigned long lastConnectionTime = 0;

int lastLcdUpdateSecond = 0;
int lastPostDataMinute = 0;
int lastPostDataDay;

// サンプリングデータ
WattSample sum_post1;
WattSample sum_post2;

WattSample sum_lcd1;
WattSample sum_lcd2;

float Watt1;
float Watt2;
float Watt_hour;

// ポストデータ
String postData;

void setup()
{
  lcd.begin(20,4);
  lcd.print(F("Power Meter"));

  bigf.attach(&lcd);

  Serial.begin(115200);
  delay(3000);

  // DHCPでIPアドレス取得
  Ethernet.begin(macAddress);

  Serial.println(F("A DHCP lease has been obtained."));

  Serial.print(F("My IP address is "));
  Serial.println(Ethernet.localIP());

  Serial.print(F("Gateway IP address is "));
  Serial.println(Ethernet.gatewayIP());

  Serial.print(F("DNS IP address is "));
  Serial.println(Ethernet.dnsServerIP());

  // 時刻初期化
  setTime(9,00,0,1,1,2012);
  ntpInit();

  // ポスト用バッファ準備
  postData.reserve(144);

  lastPostDataMinute = minute();
  lastPostDataDay = day();
  Watt_hour = 0;
}

void loop()
{
  WattSample sample1;
  WattSample sample2;

  // NTP同期
  ntpPolling();

  // ホストからのデータ受信
  while (client.available() > 0) {
    char c = client.read();
    Serial.print(c);
  }
  if ((millis() - lastConnectionTime) > 5000) {
    if (client.connected()) {
      Serial.println(F("Disconnecting."));
      client.stop();
    }
  }

  // パワーライン1と2の電力計測
  calcWatt(sample1, ct1Pin, vt1Pin);
  calcWatt(sample2, ct2Pin, vt2Pin);

  // 計測結果を積算する
  addSample(sum_lcd1, sample1);  
  addSample(sum_post1, sample1);

  addSample(sum_lcd2, sample2);
  addSample(sum_post2, sample2);

  // LCDの表示を更新
  if(elapsedSecond(lastLcdUpdateSecond) >= 2){
    lastLcdUpdateSecond = second();

    //lcd.clear();
    lcd.print(meanVrms(sum_lcd1),1);
    lcd.print(F("V "));
    lcd.print(meanIrms(sum_lcd1),1);
    lcd.print(F("A "));
    lcd.print(meanWatt(sum_lcd1),1);
    lcd.print(F("W "));

    lcd.setCursor(0, 1);
    lcd.print(meanVrms(sum_lcd2),1);
    lcd.print(F("V "));
    lcd.print(meanIrms(sum_lcd2),1);
    lcd.print(F("A "));
    lcd.print(meanWatt(sum_lcd2),1);
    lcd.print(F("W "));

    bigf.setCursor(0, 2);
    bigf.print(meanWatt(sum_lcd1)+meanWatt(sum_lcd2),1);
    lcd.print("W");

    clearSample(sum_lcd1);
    clearSample(sum_lcd2);
  }

  // Pachubeにデータを送信
  if(elapsedMinute(lastPostDataMinute) >= updateIntervalInMinutes){
    lastPostDataMinute = minute();

    Serial.println();
    Serial.println(F("Post data..."));

    Watt1 = meanWatt(sum_post1);
    Watt2 = meanWatt(sum_post2);
    float wh = (Watt1 + Watt2) / (60 / updateIntervalInMinutes);
    if(lastPostDataDay == day()){
      Watt_hour += wh;
    }
    else{
      Watt_hour = wh;
    }
    lastPostDataDay = day();

    postDataStream();

    clearSample(sum_post1);
    clearSample(sum_post2);
  }
}

void postDataStream(void)
{
  if (client.connected()) {
    Serial.println();
    Serial.println(F("Disconnecting."));
    client.stop();
  }

  // 接続を開始
  Serial.println();
  Serial.print(F("Connecting to Pachube..."));

  if (client.connect("www.pachube.com", 80)) {
    Serial.println(F("connected"));
    lastConnectionTime = millis();
  }
  else {
    Serial.println(F("failed"));
    return;
  }

  postData = "";

  // 0	Watt		Watt (W)
  postData += "0,";
  appendFloatValueAsString(postData, Watt1);
  postData += "\n";

  // 1	Vrms		volts root mean square (Vrms)
  postData += "1,";
  appendFloatValueAsString(postData, meanVrms(sum_post1));
  postData += "\n";

  // 2	Irms		ampere root mean square (Irms)
  postData += "2,";
  appendFloatValueAsString(postData, meanIrms(sum_post1));
  postData += "\n";

  // 3	Watt		Watt (W)
  postData += "3,";
  appendFloatValueAsString(postData, Watt2);
  postData += "\n";

  // 4	Vrms		volts root mean square (Vrms)
  postData += "4,";
  appendFloatValueAsString(postData, meanVrms(sum_post2));
  postData += "\n";

  // 5	Irms		ampere root mean square (Irms)
  postData += "5,";
  appendFloatValueAsString(postData, meanIrms(sum_post2));
  postData += "\n";

  // 6	Watt-hour		watt-hour (Wh)
  postData += "6,";
  appendFloatValueAsString(postData, Watt_hour);
  postData += "\n";

  //  // 100	Watt		Watt (W)
  //  postData += "100,";
  //  appendFloatValueAsString(postData, Watt1);
  //  postData += "\n";
  //
  //  // 101	Watt		Watt (W)
  //  postData += "101,";
  //  appendFloatValueAsString(postData, Watt2);
  //  postData += "\n";
  //
  //  // 102	Watt-total	Watt (W)
  //  postData += "102,";
  //  appendFloatValueAsString(postData, Watt1 + Watt2);
  //  postData += "\n";
  //
  //  // 103	Watt-hour	watt-hour (Wh)
  //  postData += "103,";
  //  appendFloatValueAsString(postData, Watt_hour);
  //  postData += "\n";

  Serial.println(postData);

  client.print(F("PUT /v2/feeds/"));
  client.print(F(ENVIRONMENT_ID));
  client.println(F(" HTTP/1.1"));
  client.println(F("User-Agent: Arduino"));
  client.println(F("Host: api.pachube.com"));
  client.print(F("X-PachubeApiKey: "));
  client.println(F(API_KEY));
  client.print(F("Content-Length: "));
  client.println(postData.length());
  client.println(F("Content-Type: text/csv"));
  client.println();
  client.println(postData);
}

