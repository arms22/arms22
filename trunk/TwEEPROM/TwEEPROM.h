#ifndef TwEEPROM_H
#define TwEEPROM_H

#include <inttypes.h>

#define TwEEPROM_WRITE_CYCLE 5

class TwEEPROMClass
{
public:
	TwEEPROMClass();
	~TwEEPROMClass(){}
	
	uint8_t read(uint8_t devaddr,uint16_t memaddr);
	void write(uint8_t devaddr,uint16_t memaddr,uint8_t data);
	
	uint8_t read(uint32_t addr);
	void write(uint32_t addr,uint8_t data);
	
	void readData(uint32_t addr,uint8_t *data,uint32_t size);
	void writeBuffer(uint32_t addr,uint8_t *buffer,uint32_t size);
};

extern TwEEPROMClass TwEEPROM;

#endif
