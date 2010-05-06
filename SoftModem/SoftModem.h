#ifndef SoftModem_h
#define SoftModem_h

#include <Print.h>
#include <inttypes.h>

//#define SOFT_MODEM_BAUD_RATE   (100)
//#define SOFT_MODEM_LOW_FREQ    (800)
//#define SOFT_MODEM_HIGH_FREQ   (1600)
//#define SOFT_MODEM_MAX_RX_BUFF (4)

//#define SOFT_MODEM_BAUD_RATE   (300)
//#define SOFT_MODEM_LOW_FREQ    (1200)
//#define SOFT_MODEM_HIGH_FREQ   (2400)
//#define SOFT_MODEM_MAX_RX_BUFF (8)

// #define SOFT_MODEM_BAUD_RATE   (600)
// #define SOFT_MODEM_LOW_FREQ    (2666)
// #define SOFT_MODEM_HIGH_FREQ   (4000)
// #define SOFT_MODEM_MAX_RX_BUFF (16)

// #define SOFT_MODEM_BAUD_RATE   (600)
// #define SOFT_MODEM_LOW_FREQ    (1800)
// #define SOFT_MODEM_HIGH_FREQ   (3600)
// #define SOFT_MODEM_MAX_RX_BUFF (16)

 #define SOFT_MODEM_BAUD_RATE   (1200)
 #define SOFT_MODEM_LOW_FREQ    (4800)
 #define SOFT_MODEM_HIGH_FREQ   (7200)
 #define SOFT_MODEM_MAX_RX_BUFF (32)

#define SOFT_MODEM_DEBUG       (1)

class SoftModem : public Print
{
private:
  volatile uint8_t *_txPortReg;
  uint8_t _txPortMask;
  uint8_t _lastTCNT;
  uint8_t _lastDiff;
  uint8_t _rawBits;
  uint8_t _errs;
  uint8_t _recvStat;
  uint8_t _recvBits;
  uint8_t _recvBufferHead;
  uint8_t _recvBufferTail;
  uint8_t _recvBuffer[SOFT_MODEM_MAX_RX_BUFF];
  void modulate(uint8_t b);
public:
  SoftModem();
  ~SoftModem();
  void begin(void);
  void end(void);
  uint8_t available(void);
  int read(void);
  void write(uint8_t data);
  void demodulate(void);
  void recv(void);
  static SoftModem *activeObject;
#if SOFT_MODEM_DEBUG
  void handleAnalogComp(bool high);
  void demodulateTest(void);
#endif
  uint8_t errs;
};

#endif
