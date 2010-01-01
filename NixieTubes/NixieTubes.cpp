#include <WProgram.h>
#include "NixieTubes.h"

NixieTubes::NixieTubes(uint8_t anod_1, uint8_t anod_2,
                       uint8_t anod_3, uint8_t anod_4,
	                   uint8_t anod_5, uint8_t anod_6,
                       uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d)
{
  uint8_t anodes[] = {anod_1,anod_2,anod_3,anod_4,anod_5,anod_6,0xff};
  init(anodes,cath_a,cath_b,cath_c,cath_d);
}

NixieTubes::NixieTubes(uint8_t anodes[],
                       uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d)
{
  init(anodes,cath_a,cath_b,cath_c,cath_d);  
}

void NixieTubes::init(uint8_t anodes[],uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d)
{
  int cnt;
  for(cnt=0; anodes[cnt]!=0xff; cnt++){
    _anodePins[cnt] = anodes[cnt];
    _numericData[cnt] = 0;
  }
  _numberOfTubes = cnt;
  _cathodePins[0] = cath_a;
  _cathodePins[1] = cath_b;
  _cathodePins[2] = cath_c;
  _cathodePins[3] = cath_d;
  _currentTube = 0;
}

void NixieTubes::begin(void)
{
  for(int i=0; i<_numberOfTubes; i++){
    pinMode(_anodePins[i], OUTPUT);
    digitalWrite(_anodePins[i], LOW);
  }
  for(int i=0; i<4; i++){
    pinMode(_cathodePins[i], OUTPUT);
    digitalWrite(_cathodePins[i], LOW);
  }
}

void NixieTubes::write(uint8_t tube,uint8_t value)
{
  _numericData[tube] = value;
}

void NixieTubes::anodeOn(void)
{
  digitalWrite(_anodePins[_currentTube], HIGH);
}

void NixieTubes::anodeOff(void)
{
  digitalWrite(_anodePins[_currentTube], LOW);
}

void NixieTubes::updateTube(void)
{
  uint8_t data;
  _currentTube++;
  if(_currentTube >= _numberOfTubes)
    _currentTube = 0;
  data = _numericData[_currentTube];
  digitalWrite(_cathodePins[0], data&1);
  digitalWrite(_cathodePins[1], data&2);
  digitalWrite(_cathodePins[2], data&4);
  digitalWrite(_cathodePins[3], data&8);
}

void NixieTubes::update(void)
{
  anodeOff();
  delayMicroseconds(ANODE_TURN_OFF_MICROS);
  updateTube();
  anodeOn();
}
