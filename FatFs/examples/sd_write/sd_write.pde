#include <FatFs.h>

const char file_name[] = "analog.txt";
File file;

void setup()
{
  delay(1000);

  if(FatFs.initialize()){
    FatFs.createFile(file_name);
  }
  else{
    while(1);
  }
}

void loop()
{
  if(file.open(file_name)){
    file.seekToEnd();
    file.println(analogRead(0));
    file.close();
  }
  delay(1000);
}


#if FAT_DATETIME_SUPPORT
void get_datetime(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min, uint8_t* sec)
{
  *year = 2010;
  *month = 1;
  *day = 23;
  *hour = 9;
  *min = 0;
  *sec = 0;
}
#endif



