// UDPクライアント
EthernetUDP Udp;

// UDPローカルポート番号
unsigned int localPort = 8888;

// NTPタイムサーバIPアドレス
IPAddress timeServer(133, 243, 238, 164); // ntp.nict.jp NTP server

// NTPパケットバッファサイズ
const int NTP_PACKET_SIZE= 48;

// NTPパケットバッファ（送受信用）
byte packetBuffer[NTP_PACKET_SIZE];

// 最後にパケットを送信した時間
unsigned long lastNtpRequestTime = 0;

void ntpInit(void)
{
  Udp.begin(localPort);

  // 最初の時刻リクエストを送信
  ntpSendPacket(timeServer);
  lastNtpRequestTime = millis();
}

void ntpPolling(void)
{
  if ( (millis() - lastNtpRequestTime) > (timeSyncIntervalInSeconds * 1000UL) ){
    // NTPサーバへ時刻リクエストを送信
    ntpSendPacket(timeServer);
    // 時間を更新
    lastNtpRequestTime = millis();
  }

  // NTPサーバからのパケット受信
  if ( Udp.parsePacket() ) {  

    // バッファに受信データを読み込む
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // 時刻情報はパケットの40バイト目からはじまる4バイトのデータ
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    // NTPタイムスタンプは64ビットの符号無し固定小数点数（整数部32ビット、小数部32ビット）
    // 1900年1月1日0時との相対的な差を秒単位で表している
    // 小数部は切り捨てて、秒を求めている
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print(F("Seconds since Jan 1 1900 = "));
    Serial.println(secsSince1900);

    // NTPタイムスタンプをUNIXタイムに変換する
    // UNITタイムは1970年1月1日0時からはじまる
    // 1900年から1970年の70年を秒で表すと2208988800秒になる
    const unsigned long seventyYears = 2208988800UL;
    // NTPタイムスタンプから70年分の秒を引くとUNIXタイムが得られる
    unsigned long epoch = secsSince1900 - seventyYears;  
    Serial.print(F("Unix time = "));
    Serial.println(epoch);

    // Timeライブラリに時間を設定(UNIXタイム)
    // 日本標準時にあわせるために＋9時間しておく
    setTime(epoch + (9 * 60 * 60));

    Serial.print(F("JST is "));
    Serial.print(year());
    Serial.print('/');
    Serial.print(month());
    Serial.print('/');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(hour());
    Serial.print(':'); 
    Serial.print(minute());
    Serial.print(':'); 
    Serial.println(second());
    Serial.println();
  }
}

// send an NTP request to the time server at the given address 
void ntpSendPacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  // NTP requests are to port 123
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

