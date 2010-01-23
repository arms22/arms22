#include <FatFs.h>

File file;

void setup()
{
  Serial.begin(9600);

  delay(1000);

  if(FatFs.initialize()){
    if(file.open("analog.txt")){
      char buf[32];
      int sz;
      while((sz = file.read((uint8_t*)buf, sizeof(buf))) > 0){
        for(int i=0; i<sz; i++){
          Serial.print(buf[i]);
        }
      }
      file.close();
    }else{
      Serial.println("can not open.");
    }
  }
  else{
    Serial.println("can not initialize.");
    while(1);
  }
}

void loop()
{
}



