#include <SPI.h>
#include <SPISRAM.h>

/*
 SRAM   Arduino
 1 CS   10(CS)
 2 SO   12(MISO)
 3 -    -
 4 Vss  GND
 5 SI   11(MOSI)
 6 SCK  13(SCK)
 7 HOLD <-- 100k ohm -- 3.3V
 8 Vcc  3.3V
 */

SPISRAM myRAM(10); // CS pin
char buffer[128];

void setup()
{
  Serial.begin(9600);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);

  Serial.println("Byte write...");
  myRAM.write(0,'H');
  myRAM.write(1,'e');
  myRAM.write(2,'l');
  myRAM.write(3,'l');
  myRAM.write(4,'o');
  myRAM.write(5,'-');

  Serial.println("Byte read...");
  Serial.println(myRAM.read(0));
  Serial.println(myRAM.read(1));
  Serial.println(myRAM.read(2));
  Serial.println(myRAM.read(3));
  Serial.println(myRAM.read(4));
  Serial.println(myRAM.read(5));

  Serial.println("\nByte write...");
  myRAM.write(0x7FFC,'W');
  myRAM.write(0x7FFD,'o');
  myRAM.write(0x7FFE,'r');
  myRAM.write(0x7FFF,'l');
  myRAM.write(0x8000,'d');
  myRAM.write(0x8001,'!');
  myRAM.write(0x8002,'!');

  Serial.println("Byte read...");
  Serial.println(myRAM.read(0x7FFC));
  Serial.println(myRAM.read(0x7FFD));
  Serial.println(myRAM.read(0x7FFE));
  Serial.println(myRAM.read(0x7FFF));
  Serial.println(myRAM.read(0x8000));
  Serial.println(myRAM.read(0x8001));
  Serial.println(myRAM.read(0x8002));

  Serial.println("\nseq write...");
  int addr = 0x7F00;
  myRAM.write(addr, (byte*)"Hello world!!", sizeof("Hello world!!"));

  Serial.println("seq read...");
  myRAM.read(addr, (byte*)buffer, sizeof(buffer));
  Serial.println( buffer );

  Serial.println("\nByte read at operator[]...");
  myRAM[100] = 'H';
  myRAM[101] = 'e';
  myRAM[102] = 'l';
  myRAM[103] = 'l';
  myRAM[104] = 'o';
  Serial.println( myRAM[100] );
  Serial.println( myRAM[101] );
  Serial.println( myRAM[102] );
  Serial.println( myRAM[103] );
  Serial.println( myRAM[104] );

  Serial.println("\nRandom read/write...");
  for(int i=0;i<100;i++){
    int addr = random() & 0x7fff;
    byte val = i;
    Serial.print( addr, HEX );
    Serial.print( " " );
    Serial.print( val, HEX );
    myRAM[addr] = val;
    Serial.print( " " );
    Serial.println( myRAM[addr], HEX );
  }
}

void loop()
{
}
