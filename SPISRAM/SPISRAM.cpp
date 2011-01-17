/*
  Copyright (c) 2010 by arms22 (arms22 at gmail.com)
  Microchip 23x256 SPI SRAM library for Arduino
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include "SPISRAM.h"

// INSTRUCTION SET
#define READ	0x03			// Read data from memory
#define WRITE	0x02			// Write data to memory
#define RDSR	0x05			// Read Status register
#define WRSR	0x01			// Write Status register

// STATUS REGISTER
#define	BYTE_MODE	0x00
#define	PAGE_MODE	0x80
#define SEQ_MODE	0x40

SPISRAM::SPISRAM(byte ncsPin)
	: _ncsPin(ncsPin)
{
}

byte SPISRAM::read(unsigned int address)
{
	_buf[0] = READ;
	_buf[1] = address >> 8;
	_buf[2] = address & 0xff;
	_buf[3] = 0x00;
	return transfer(4);
}

void SPISRAM::read(unsigned int address, byte *buffer, unsigned int size)
{
	_buf[0] = WRSR;
	_buf[1] = SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(READ);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	for(unsigned int i=0; i<size; i++){
		*buffer++ = SPI.transfer(0);
	}
	deselect();
}

void SPISRAM::write(unsigned int address, byte data)
{
	_buf[0] = WRITE;
	_buf[1] = address >> 8;
	_buf[2] = address & 0xff;
	_buf[3] = data;
	transfer(4);
}

void SPISRAM::write(unsigned int address, byte *buffer, unsigned int size)
{
	_buf[0] = WRSR;
	_buf[1] = SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(WRITE);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	for(unsigned int i=0; i<size; i++){
		SPI.transfer(*buffer++);
	}
	deselect();
}

void SPISRAM::select(void)
{
	digitalWrite(_ncsPin, LOW);
}

void SPISRAM::deselect(void)
{
	digitalWrite(_ncsPin, HIGH);
}

byte SPISRAM::transfer(byte length)
{
	byte ret,i;
	select();
	for(i=0; i<length; i++){
		ret = SPI.transfer(_buf[i]);
	}
	deselect();
	return ret;
}
