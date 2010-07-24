#include <WProgram.h>
#include <WConstants.h>
#include <avr/interrupt.h>
#include <pins_arduino.h>
#include "SoftModem.h"

#define SOFT_MODEM_TX_PIN      (3)
#define SOFT_MODEM_RX_PIN1     (6)  // AIN0
#define SOFT_MODEM_RX_PIN2     (7)  // AIN1

SoftModem *SoftModem::activeObject = 0;

SoftModem::SoftModem() {
}

SoftModem::~SoftModem() {
	end();
}

#if F_CPU == 16000000
#if SOFT_MODEM_BAUD_RATE <= 126
  #define TIMER_CLOCK_SELECT       (7)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(1024))
#elif SOFT_MODEM_BAUD_RATE <= 315
  #define TIMER_CLOCK_SELECT       (6)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(256))
#elif SOFT_MODEM_BAUD_RATE <= 630
  #define TIMER_CLOCK_SELECT       (5)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(128))
#elif SOFT_MODEM_BAUD_RATE <= 1225
  #define TIMER_CLOCK_SELECT       (4)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(64))
#else
  #define TIMER_CLOCK_SELECT       (3)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(32))
#endif
#else
#if SOFT_MODEM_BAUD_RATE <= 126
  #define TIMER_CLOCK_SELECT       (6)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(256))
#elif SOFT_MODEM_BAUD_RATE <= 315
  #define TIMER_CLOCK_SELECT       (5)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(128))
#elif SOFT_MODEM_BAUD_RATE <= 630
  #define TIMER_CLOCK_SELECT       (4)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(64))
#else
  #define TIMER_CLOCK_SELECT       (3)
  #define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(32))
#endif
#endif

#define SOFT_MODEM_BIT_PERIOD      (1000000/SOFT_MODEM_BAUD_RATE)
#define SOFT_MODEM_HIGH_USEC       (1000000/SOFT_MODEM_HIGH_FREQ)
#define SOFT_MODEM_LOW_USEC        (1000000/SOFT_MODEM_LOW_FREQ)

#define SOFT_MODEM_HIGH_CNT        (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_HIGH_USEC)
#define SOFT_MODEM_LOW_CNT         (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_LOW_USEC)

#define SOFT_MODEM_HIGH_ADJ        (SOFT_MODEM_BIT_PERIOD%SOFT_MODEM_HIGH_USEC)
#define SOFT_MODEM_LOW_ADJ         (SOFT_MODEM_BIT_PERIOD%SOFT_MODEM_LOW_USEC)

#define SOFT_MODEM_CARRIR_CNT      (20000000/SOFT_MODEM_BIT_PERIOD)

#define TCNT_BIT_PERIOD            (SOFT_MODEM_BIT_PERIOD/MICROS_PER_TIMER_COUNT)
#define TCNT_HIGH_FREQ             (SOFT_MODEM_HIGH_USEC/MICROS_PER_TIMER_COUNT)
#define TCNT_LOW_FREQ              (SOFT_MODEM_LOW_USEC/MICROS_PER_TIMER_COUNT)

#define TCNT_HIGH_TH_L             (TCNT_HIGH_FREQ * 0.80)
#define TCNT_HIGH_TH_H             (TCNT_HIGH_FREQ * 1.15)
#define TCNT_LOW_TH_L              (TCNT_LOW_FREQ * 0.85)
#define TCNT_LOW_TH_H              (TCNT_LOW_FREQ * 1.20)

#if SOFT_MODEM_DEBUG
static volatile uint8_t *portLEDReg;
static uint8_t portLEDMask;
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

#if SOFT_MODEM_DEBUG
	portLEDReg = portOutputRegister(digitalPinToPort(13));
	portLEDMask = digitalPinToBitMask(13);
	_errs = 0;
	_ints = 0;
#endif

	_recvStat = 0xff;
	_recvBufferHead = _recvBufferTail = 0;

	SoftModem::activeObject = this;

	_lastTCNT = TCNT2;
	_lastDiff = _lowCount = _highCount = 0;

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

enum {
	FSK_START_BIT = 0,
	FSK_D0_BIT,
	FSK_D1_BIT,
	FSK_D2_BIT,
	FSK_D3_BIT,
	FSK_D4_BIT,
	FSK_D5_BIT,
	FSK_D6_BIT,
	FSK_D7_BIT,  
	FSK_STOP_BIT
};

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
	
#if SOFT_MODEM_DEBUG
	_ints++;
#endif
	
	if(diff < (uint8_t)(TCNT_HIGH_TH_L))				// Noise?
		return;
	
	_lastTCNT = t;
	
	if(diff > (uint8_t)(TCNT_LOW_TH_H))
		return;
	
	_lastDiff = (diff >> 1) + (diff >> 2) + (_lastDiff >> 2);
	
	if(_lastDiff >= (uint8_t)(TCNT_LOW_TH_L)){
		_lowCount += _lastDiff;
		if((_recvStat == 0xff) && (_lowCount >= (uint8_t)(TCNT_BIT_PERIOD * 0.5))){ // maybe Start-Bit
			_recvStat  = FSK_START_BIT;
			_highCount = 0;
			_recvBits  = 0;
			OCR2A      = t + (uint8_t)(TCNT_BIT_PERIOD) - _lowCount; // 1 bit period after detected
			TIFR2     |= _BV(OCF2A);
			TIMSK2    |= _BV(OCIE2A);
		}
	}
	else if(_lastDiff <= (uint8_t)(TCNT_HIGH_TH_H)){
		_highCount += _lastDiff;
		if((_recvStat == 0xff) && (_highCount >= (uint8_t)(TCNT_BIT_PERIOD))){
			_lowCount = _highCount = 0;
		}
	}
	else{
#if SOFT_MODEM_DEBUG
		_errs++;
#endif
	}
}

ISR(ANALOG_COMP_vect)
{
	SoftModem *act = SoftModem::activeObject;
	act->demodulate();
}

void SoftModem::recv(void)
{
	uint8_t high;
	if(_highCount > _lowCount){
 		if(_highCount >= (uint8_t)TCNT_BIT_PERIOD)
 			_highCount -= (uint8_t)TCNT_BIT_PERIOD;
		else
			_highCount = 0;
		high = 0x80;
	}
	else{
 		if(_lowCount >= (uint8_t)TCNT_BIT_PERIOD)
 			_lowCount -= (uint8_t)TCNT_BIT_PERIOD;
 		else
			_lowCount = 0;
		high = 0x00;
	}
	
	if(_recvStat == FSK_START_BIT){	// Start bit
		if(!high){
			_recvStat++;
		}else{
			goto end_recv;
		}
	}
	else if(_recvStat <= FSK_D7_BIT) { // Data bits
		_recvBits >>= 1;
		_recvBits |= high;
		_recvStat++;
	}
	else if(_recvStat == FSK_STOP_BIT){	// Stop bit
		uint8_t new_tail = (_recvBufferTail + 1) & (SOFT_MODEM_MAX_RX_BUFF - 1);
		if(new_tail != _recvBufferHead){
			_recvBuffer[_recvBufferTail] = _recvBits;
			_recvBufferTail = new_tail;
		}
		goto end_recv;
	}
	else{
	end_recv:
		_recvStat = 0xff;
		TIMSK2 &= ~_BV(OCIE2A);
#if SOFT_MODEM_DEBUG
		errs = _errs;
		_errs = 0;
		ints = _ints;
		_ints = 0;
#endif
	}
}

ISR(TIMER2_COMPA_vect)
{
	OCR2A += (uint8_t)TCNT_BIT_PERIOD;
	SoftModem *act = SoftModem::activeObject;
	act->recv();
#if SOFT_MODEM_DEBUG
	*portLEDReg ^= portLEDMask;
#endif  
}

uint8_t SoftModem::available(void)
{
	return (_recvBufferTail + SOFT_MODEM_MAX_RX_BUFF - _recvBufferHead) & (SOFT_MODEM_MAX_RX_BUFF - 1);
}

int SoftModem::read(void)
{
	if(_recvBufferHead == _recvBufferTail)
		return -1;
	int d = _recvBuffer[_recvBufferHead];
	_recvBufferHead = (_recvBufferHead + 1) & (SOFT_MODEM_MAX_RX_BUFF - 1);
	return d;
}

void SoftModem::modulate(uint8_t b)
{
	uint8_t cnt,tcnt,tcnt2,adj;
	if(b){
		cnt = (uint8_t)(SOFT_MODEM_HIGH_CNT);
		tcnt2 = (uint8_t)(TCNT_HIGH_FREQ / 2);
		tcnt = (uint8_t)(TCNT_HIGH_FREQ) - tcnt2;
	}else{
		cnt = (uint8_t)(SOFT_MODEM_LOW_CNT);
		tcnt2 = (uint8_t)(TCNT_LOW_FREQ / 2);
		tcnt = (uint8_t)(TCNT_LOW_FREQ) - tcnt2;
	}
	do {
		cnt--;
		{
			OCR2B += tcnt;
			TIFR2 |= _BV(OCF2B);
			while(!(TIFR2 & _BV(OCF2B)));
		}
		*_txPortReg ^= _txPortMask;
		{
			OCR2B += tcnt2;
			TIFR2 |= _BV(OCF2B);
			while(!(TIFR2 & _BV(OCF2B)));
		}
		*_txPortReg ^= _txPortMask;
	} while (cnt);
}

void SoftModem::write(uint8_t data)
{
	static unsigned long lastTransmissionTime = 0;
	if((micros() - lastTransmissionTime) > (uint16_t)(SOFT_MODEM_LOW_USEC*2)){
		for(uint8_t i = 0; i<(uint8_t)SOFT_MODEM_CARRIR_CNT; i++){
			modulate(HIGH);
		}
	}
	modulate(LOW);							 // Start Bit
	for(uint8_t mask = 1; mask; mask <<= 1){ // Data Bits
		if(data & mask){
			modulate(HIGH);
		}
		else{
			modulate(LOW);
		}
	}
	modulate(HIGH);				// Stop Bit
	modulate(HIGH);				// Push Bit
	lastTransmissionTime = micros();
}

#if SOFT_MODEM_DEBUG
#include <HardwareSerial.h>

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
	Serial.println(TCNT_LOW_FREQ,DEC);
	Serial.println(TCNT_LOW_TH_L,DEC);
	Serial.println(TCNT_LOW_TH_H,DEC);

	Serial.println("high freq TMC > ");
	Serial.println(TCNT_HIGH_FREQ,DEC);
	Serial.println(TCNT_HIGH_TH_L,DEC);
	Serial.println(TCNT_HIGH_TH_H,DEC);

	Serial.print("bit period TMC = ");
	Serial.println(TCNT_BIT_PERIOD,DEC);

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
