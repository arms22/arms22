#include <WProgram.h>
#include <WConstants.h>
#include <avr/interrupt.h>
#include <pins_arduino.h>
#include "SoftModem.h"

// Low speed
//  LOW frequency: 800Hz
//  HIGH frequency: 1600Hz
//  Data transfer rate: 100 baud
//  Brief carrier tone before each transmission (HIGH)
//  1 start bit (LOW)
//  8 data bits, LSB first
//  1 even parity bit
//  1 stop bit (HIGH)

// High speed
//  LOW frequency: 2666Hz
//  HIGH frequency: 4000Hz
//  Data transfer rate: 600 baud
//  Brief carrier tone before each transmission (HIGH)
//  1 start bit (LOW)
//  8 data bits, LSB first
//  1 even parity bit
//  1 stop bit (HIGH)

#define SOFT_MODEM_TX_PIN      (3)
#define SOFT_MODEM_RX_PIN1     (6)  // AIN0
#define SOFT_MODEM_RX_PIN2     (7)  // AIN1

SoftModem *SoftModem::activeObject = 0;

SoftModem::SoftModem() {
}

SoftModem::~SoftModem() {
  end();
}

#if SOFT_MODEM_BAUD_RATE <= 100
  #define TIMER_CLOCK_SELECT		(7) // clk/1024
  #define MICROS_PER_TIMER_COUNT	(clockCyclesToMicroseconds(1024))
#elif SOFT_MODEM_BAUD_RATE <= 600
  #define TIMER_CLOCK_SELECT		(5) // clk/128
  #define MICROS_PER_TIMER_COUNT	(clockCyclesToMicroseconds(128))
#else
  #define TIMER_CLOCK_SELECT		(4) // clk/64
  #define MICROS_PER_TIMER_COUNT	(clockCyclesToMicroseconds(64))
#endif

#if SOFT_MODEM_DEBUG
volatile uint8_t *portLEDReg;
uint8_t portLEDMask;
#endif

void SoftModem::begin(void)
{
  pinMode(SOFT_MODEM_RX_PIN1, INPUT);
  digitalWrite(SOFT_MODEM_RX_PIN1, LOW);

  pinMode(SOFT_MODEM_RX_PIN2, INPUT);
  digitalWrite(SOFT_MODEM_RX_PIN2, LOW);

  pinMode(SOFT_MODEM_TX_PIN, OUTPUT);
  digitalWrite(SOFT_MODEM_TX_PIN, LOW);

  _txPortReg = portOutputRegister(digitalPinToPort(SOFT_MODEM_TX_PIN));
  _txPortMask = digitalPinToBitMask(SOFT_MODEM_TX_PIN);

  _rawBits = _recvStat = 0xff;
  _recvBufferHead = _recvBufferTail = 0;

#if SOFT_MODEM_DEBUG
  portLEDReg = portOutputRegister(digitalPinToPort(13));
  portLEDMask = digitalPinToBitMask(13);
#endif

  SoftModem::activeObject = this;

  _lastTCNT = TCNT2;
  _lastDiff = 0;
  TCCR2A = 0;
  TCCR2B = TIMER_CLOCK_SELECT;
  ACSR   = _BV(ACIE) | _BV(ACIS1);
}

void SoftModem::end(void)
{
  ACSR   &= ~(_BV(ACIE));
  TIMSK2 &= ~(_BV(OCIE2A));
  SoftModem::activeObject = 0;
}

#define SOFT_MODEM_BIT_PERIOD      (1000000/SOFT_MODEM_BAUD_RATE)
#define SOFT_MODEM_LOW_USEC        (1000000/SOFT_MODEM_LOW_FREQ)
#define SOFT_MODEM_HIGH_USEC       (1000000/SOFT_MODEM_HIGH_FREQ)
#define TIMER_COUNT_PER_BIT_PERIOD (SOFT_MODEM_BIT_PERIOD/MICROS_PER_TIMER_COUNT)
#define TIMER_COUNT_PER_LOW_FREQ   (SOFT_MODEM_LOW_USEC/MICROS_PER_TIMER_COUNT)
#define TIMER_COUNT_PER_HIGH_FREQ  (SOFT_MODEM_HIGH_USEC/MICROS_PER_TIMER_COUNT)

void SoftModem::demodulate(void)
{
  uint8_t t = TCNT2;
  uint8_t diff;

  if(TIFR2 & _BV(TOV2)){
    TIFR2 |= _BV(TOV2);
    diff = (255 - _lastTCNT) + t + 1;
  }
  else{
    diff = t - _lastTCNT;
  }

  if(diff < 3)
    return;

  _lastTCNT = t;
//  _lastDiff = diff = ((diff >> 1) + (diff >> 2) + (_lastDiff >> 2));

  if((diff >= (uint8_t)(TIMER_COUNT_PER_LOW_FREQ * 0.8)) &&
     (diff <= (uint8_t)(TIMER_COUNT_PER_LOW_FREQ * 1.2))){
    _rawBits <<= 1;
    if(_recvStat == 0xff && !(_rawBits & 3)){
      _recvStat = _recvBits = 0;
      OCR2A     = t + (uint8_t)(TIMER_COUNT_PER_BIT_PERIOD + (TIMER_COUNT_PER_BIT_PERIOD>>1) - (TIMER_COUNT_PER_LOW_FREQ<<1));
      TIFR2    |= _BV(OCF2A);
      TIMSK2   |= _BV(OCIE2A);
    }
  }
  else if((diff >= (uint8_t)(TIMER_COUNT_PER_HIGH_FREQ * 0.8)) &&
          (diff <= (uint8_t)(TIMER_COUNT_PER_HIGH_FREQ * 1.2))){
    _rawBits <<= 1;
    _rawBits |= 1;
  }
  else{
    _errs++;
  }
}

ISR(ANALOG_COMP_vect)
{
  SoftModem *act = SoftModem::activeObject;
  if(act){
    act->demodulate();
  }
}

void SoftModem::recv(void)
{
  if(_recvStat < 8){            // Data Bits
    _recvBits |= (_rawBits & 1) << _recvStat;
    _recvStat++;
  }
  else if(_recvStat == 8){      // Parity Bit
    _recvStat++;
  }
  else if(_recvStat == 9){      // Stop Bit
    uint8_t new_tail = (_recvBufferTail + 1) % SOFT_MODEM_MAX_RX_BUFF;
    if(new_tail != _recvBufferHead){
      _recvBuffer[_recvBufferTail] = _recvBits;
      _recvBufferTail = new_tail;
    }
    _rawBits = _recvStat = 0xff;
    errs = _errs;
	_errs = 0;
    TIMSK2 &= ~_BV(OCIE2A);
  }
}

ISR(TIMER2_COMPA_vect)
{
  OCR2A += (uint8_t)TIMER_COUNT_PER_BIT_PERIOD;
  SoftModem *act = SoftModem::activeObject;
  if(act){
    act->recv();
  }
#if SOFT_MODEM_DEBUG
  *portLEDReg ^= portLEDMask;
#endif	
}

uint8_t SoftModem::available(void)
{
  return (_recvBufferTail + SOFT_MODEM_MAX_RX_BUFF - _recvBufferHead) % SOFT_MODEM_MAX_RX_BUFF;
}

int SoftModem::read(void)
{
  if(_recvBufferHead == _recvBufferTail)
    return -1;
  int d = _recvBuffer[_recvBufferHead];
  _recvBufferHead = (_recvBufferHead + 1) % SOFT_MODEM_MAX_RX_BUFF;
  return d;
}

void SoftModem::modulate(uint8_t b)
{
  uint16_t half;
  TIFR2|= _BV(OCF2B);

  if(b)
    half = (uint16_t)(SOFT_MODEM_HIGH_USEC/2);
  else
    half = (uint16_t)(SOFT_MODEM_LOW_USEC/2);
  do {
    delayMicroseconds(half);
    *_txPortReg ^= _txPortMask;
//#if SOFT_MODEM_DEBUG
//    *portLEDReg ^= portLEDMask;
//#endif
  } while (!(TIFR2 & _BV(OCF2B)));

  OCR2B += (uint8_t)TIMER_COUNT_PER_BIT_PERIOD;
}

void SoftModem::write(uint8_t data)
{
  uint8_t parity = 0;
  OCR2B = TCNT2 + (uint8_t)TIMER_COUNT_PER_BIT_PERIOD;
  modulate(HIGH);						   // Synchronization Bit
  modulate(LOW);						   // Start Bit
  for(uint8_t mask = 1; mask; mask <<= 1){ // Data Bits
    if(data & mask){
      parity++;
      modulate(HIGH);
    }
    else{
      modulate(LOW);
    }
  }
  modulate(parity & 1);      // Parity Bit
  modulate(HIGH);			 // Stop Bit
  modulate(HIGH);			 // Dammy Bit
}

#if SOFT_MODEM_DEBUG

#include <HardwareSerial.h>

#define SOFT_MODEM_LOW_CNT  (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_LOW_USEC)
#define SOFT_MODEM_HIGH_CNT (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_HIGH_USEC)
#define SOFT_MODEM_LOW_ADJ  (SOFT_MODEM_BIT_PERIOD%SOFT_MODEM_LOW_USEC)
#define SOFT_MODEM_HIGH_ADJ (SOFT_MODEM_BIT_PERIOD%SOFT_MODEM_HIGH_USEC)

void SoftModem::handleAnalogComp(bool high)
{
  int cnt = (high ? SOFT_MODEM_HIGH_CNT : SOFT_MODEM_LOW_CNT);
  int usec = (high ? SOFT_MODEM_HIGH_USEC : SOFT_MODEM_LOW_USEC);
  int adj = (high ? SOFT_MODEM_HIGH_ADJ : SOFT_MODEM_LOW_ADJ);
  for(int i=0;i<cnt;i++){
    unsigned long end = micros() + usec;
    demodulate();
    while(micros() < end);
  }
  if(adj)
    delayMicroseconds(adj);
}

void SoftModem::demodulateTest(void)
{
  Serial.begin(115200);

  Serial.print("bit period = ");
  Serial.println(SOFT_MODEM_BIT_PERIOD);

  Serial.print("low usec = ");
  Serial.println(SOFT_MODEM_LOW_USEC);

  Serial.print("high usec = ");
  Serial.println(SOFT_MODEM_HIGH_USEC);

  Serial.print("low cnt = ");
  Serial.println(SOFT_MODEM_LOW_CNT);

  Serial.print("high cnt = ");
  Serial.println(SOFT_MODEM_HIGH_CNT);

  Serial.print("low adj = ");
  Serial.println(SOFT_MODEM_LOW_ADJ);

  Serial.print("high adj = ");
  Serial.println(SOFT_MODEM_HIGH_ADJ);

  Serial.print("TMC micros = ");
  Serial.println(MICROS_PER_TIMER_COUNT);

  Serial.println("low freq TMC > ");
  Serial.println(TIMER_COUNT_PER_LOW_FREQ,DEC);
  Serial.println(TIMER_COUNT_PER_LOW_FREQ*0.9,DEC);
  Serial.println(TIMER_COUNT_PER_LOW_FREQ*1.1,DEC);

  Serial.println("high freq TMC > ");
  Serial.println(TIMER_COUNT_PER_HIGH_FREQ,DEC);
  Serial.println(TIMER_COUNT_PER_HIGH_FREQ*0.9,DEC);
  Serial.println(TIMER_COUNT_PER_HIGH_FREQ*1.1,DEC);

  Serial.print("bit period TMC = ");
  Serial.println(TIMER_COUNT_PER_BIT_PERIOD,DEC);

  begin();

  delay(200);

  handleAnalogComp(0);//start bit  

  handleAnalogComp(1);
  handleAnalogComp(0);
  handleAnalogComp(1);
  handleAnalogComp(0);

  handleAnalogComp(0);
  handleAnalogComp(1);
  handleAnalogComp(0);
  handleAnalogComp(1);

  handleAnalogComp(1);//parity bit
  handleAnalogComp(1);//stop bit

  delay(300);

  handleAnalogComp(0);//start bit  

  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(0);

  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(0);

  handleAnalogComp(1);//parity bit
  handleAnalogComp(1);//stop bit

  delay(300);

  handleAnalogComp(0);//start bit  

  handleAnalogComp(0);
  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(1);

  handleAnalogComp(0);
  handleAnalogComp(1);
  handleAnalogComp(1);
  handleAnalogComp(1);

  handleAnalogComp(1);//parity bit
  handleAnalogComp(1);//stop bit

  delay(300);

  Serial.println("--");
  Serial.println(_recvStat,HEX);
  Serial.println(_lastTCNT,HEX);
  Serial.println(_recvBits,HEX);

  while(available()){
    Serial.print("data=");
    Serial.println(read(),HEX);
  }

  end();
}
#endif
