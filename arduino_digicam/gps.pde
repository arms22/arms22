// GPS Baud Rate
const int gpsBaud     = 9600;

// GPS制御端子
const int gpsRxPin    = 2;
const int gpsTxPin    = 4;
const int gpsPowerPin = 8;

// GPS通信
NewSoftSerial gpsPort(gpsRxPin, gpsTxPin);

// NMEA Encorder
TinyGPS gps;

// GPS受信データ
uint8_t fetchDataLength;
char    fetchData[200];

// ファイル保存用
File    gpslog;

void gps_powerOn(void)
{
  pinMode(gpsPowerPin, OUTPUT);
  digitalWrite(gpsPowerPin, HIGH);
  sleep(200);
  gpsPort.flush();
  gpsPort.begin(gpsBaud);
  setSyncProvider(gps_timeSync);

  if( FatFs.changeDirectory("/") ){
    char filename[] = "gps.nmea";
    FatFs.createFile(filename);
    gpslog.open(filename);
    gpslog.seekToEnd();
  }
}

void gps_powerOff(void)
{
  setSyncProvider(NULL);
  gpsPort.end();
  pinMode(gpsRxPin, INPUT);
  pinMode(gpsTxPin, INPUT);
  digitalWrite(gpsPowerPin, LOW);
  sleep(200);

  gpslog.close();
}

void gps_suspend(bool suspend)
{
  if( suspend ){
    gpsPort.end();
  }else{
    gpsPort.flush();
    gpsPort.begin(gpsBaud);
  }
}

bool gps_fetch(void)
{
  if( gpsPort.available() ){
    unsigned long last_time = millis();
    uint8_t preamble = 0;
    uint8_t len = 0;
    fetchDataLength = 0;
    do {
      while( gpsPort.available() ){
        char c = gpsPort.read();
        if( c == '$' ){
          preamble++;
        } 
        if( preamble ){
          if( len < sizeof(fetchData) ){
            fetchData[len++] = c;
          }
          if( c == '\n'){
            fetchDataLength = len;
          }
        }
        last_time = millis();
      }
      sleep(1);
    }
    while( millis() - last_time < 10 );
    return fetchDataLength > 0;
  }
  return false;
}

void gps_encode(void)
{
  for(int i=0;i<fetchDataLength;i++){
    gps.encode(fetchData[i]);
  }
}

void gps_writeData(void)
{
  gpslog.write((const uint8_t *)fetchData, fetchDataLength);
}

time_t gps_timeSync()
{
  unsigned long fix_age;
  gps.get_datetime(NULL, NULL, &fix_age);

  if( fix_age < 5000 ){
    tmElements_t tm;
    int          year;
    byte         month, day, hour, minute, second;
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, NULL, NULL);
    tm.Year   = year - 1970;
    tm.Month  = month;
    tm.Day    = day;
    tm.Hour   = hour;
    tm.Minute = minute;
    tm.Second = second;
    return makeTime(tm) + (9 * SECS_PER_HOUR);
  }

  return 0;
}
