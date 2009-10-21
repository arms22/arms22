#include <WProgram.h>
#include <Wire.h>
#include "TwEEPROM.h"

TwEEPROMClass::TwEEPROMClass()
{
}

uint8_t TwEEPROMClass::read(uint8_t devaddr,uint16_t memaddr)
{
	uint8_t rdata = 0xFF;
	Wire.beginTransmission(devaddr);
	Wire.send((uint8_t)(memaddr >> 8));
	Wire.send((uint8_t)(memaddr & 0xff));
	Wire.endTransmission();
	Wire.requestFrom(devaddr,(uint8_t)1);
	if (Wire.available())
		rdata = Wire.receive();
	return rdata;
}

void TwEEPROMClass::write(uint8_t devaddr,uint16_t memaddr,uint8_t data)
{
	Wire.beginTransmission(devaddr);
	Wire.send((uint8_t)(memaddr >> 8));
	Wire.send((uint8_t)(memaddr & 0xff));
	Wire.send(data);
	Wire.endTransmission();
	delay(TwEEPROM_WRITE_CYCLE);
}

uint8_t TwEEPROMClass::read(uint32_t addr)
{
	return read(addr>>16,addr&0xffff);
}

void TwEEPROMClass::write(uint32_t addr,uint8_t data)
{
	write((addr>>16)&0xff,(addr&0xffff),data);
}

void TwEEPROMClass::readData(uint32_t addr,uint8_t *data,uint32_t size)
{
	// Todo support SEQUENTIAL READ
	for (;size>0;size--) {
		*data++ = read(addr++);
	}
}

void TwEEPROMClass::writeBuffer(uint32_t addr,uint8_t *buffer,uint32_t size)
{
	// Todo support PAGE WRITE
	for (;size;size--) {
		write(addr++,*buffer++);
	}
}

TwEEPROMClass TwEEPROM;
