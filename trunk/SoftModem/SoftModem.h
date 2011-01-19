#ifndef SoftModem_h
#define SoftModem_h

#include <Stream.h>
#include <inttypes.h>

//#define SOFT_MODEM_BAUD_RATE   (126)
//#define SOFT_MODEM_LOW_FREQ    (882)
//#define SOFT_MODEM_HIGH_FREQ   (1764)
//#define SOFT_MODEM_MAX_RX_BUFF (4)

//#define SOFT_MODEM_BAUD_RATE   (100)
//#define SOFT_MODEM_LOW_FREQ    (800)
//#define SOFT_MODEM_HIGH_FREQ   (1600)
//#define SOFT_MODEM_MAX_RX_BUFF (4)

// #define SOFT_MODEM_BAUD_RATE   (315)
// #define SOFT_MODEM_LOW_FREQ    (1575)
// #define SOFT_MODEM_HIGH_FREQ   (3150)
// #define SOFT_MODEM_MAX_RX_BUFF (8)

// #define SOFT_MODEM_BAUD_RATE   (630)
// #define SOFT_MODEM_LOW_FREQ    (3150)
// #define SOFT_MODEM_HIGH_FREQ   (6300)
// #define SOFT_MODEM_MAX_RX_BUFF (16)

//#define SOFT_MODEM_BAUD_RATE   (600)
//#define SOFT_MODEM_LOW_FREQ    (2666)
//#define SOFT_MODEM_HIGH_FREQ   (4000)
//#define SOFT_MODEM_MAX_RX_BUFF (16)

#define SOFT_MODEM_BAUD_RATE   (1225)
#define SOFT_MODEM_LOW_FREQ    (4900)
#define SOFT_MODEM_HIGH_FREQ   (7350)
#define SOFT_MODEM_MAX_RX_BUFF (32)

//#define SOFT_MODEM_BAUD_RATE   (2450)
//#define SOFT_MODEM_LOW_FREQ    (7350)
//#define SOFT_MODEM_HIGH_FREQ   (14700)
//#define SOFT_MODEM_MAX_RX_BUFF (32)

//  Brief carrier tone before each transmission
//  1 start bit (LOW)
//  8 data bits, LSB first
//  1 stop bit (HIGH)
//  1 push bit (HIGH)

#define SOFT_MODEM_DEBUG       (0)

class SoftModem : public Stream
{
private:
	volatile uint8_t *_txPortReg;
	uint8_t _txPortMask;
	uint8_t _lastTCNT;
	uint8_t _lastDiff;
	uint8_t _recvStat;
	uint8_t _recvBits;
	uint8_t _recvBufferHead;
	uint8_t _recvBufferTail;
	uint8_t _recvBuffer[SOFT_MODEM_MAX_RX_BUFF];
	uint8_t _lowCount;
	uint8_t _highCount;
	void modulate(uint8_t b);
public:
	SoftModem();
	~SoftModem();
	void begin(void);
	void end(void);
	virtual int available();
	virtual int read();
	virtual void flush();
	virtual int peek();
	virtual void write(uint8_t data);
	void demodulate(void);
	void recv(void);
	static SoftModem *activeObject;
#if SOFT_MODEM_DEBUG
	void handleAnalogComp(bool high);
	void demodulateTest(void);
	uint8_t _errs;
	uint16_t _ints;
	uint8_t errs;
	uint16_t ints;
#endif
};

#endif
