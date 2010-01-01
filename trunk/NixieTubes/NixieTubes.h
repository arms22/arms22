#ifndef NixieTubes_h
#define NixieTubes_h

#include <inttypes.h>

#define NUMBER_OF_MAXTUBES 14	/* YYYY.MM.DD hh:mm:ss */
#define ANODE_TURN_OFF_MICROS 500

class NixieTubes
{
private:
  uint8_t _anodePins[NUMBER_OF_MAXTUBES];
  uint8_t _cathodePins[4];
  uint8_t _numericData[NUMBER_OF_MAXTUBES];
  uint8_t _numberOfTubes;
  uint8_t _currentTube;
public:
  NixieTubes(uint8_t anod_1, uint8_t anod_2,
             uint8_t anod_3, uint8_t anod_4,
             uint8_t anod_5, uint8_t anod_6,
             uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d);
  
  NixieTubes(uint8_t anodes[],
             uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d);
  
  void begin(void);
  void write(uint8_t tube,uint8_t value);
  void anodeOn(void);
  void anodeOff(void);
  void updateTube(void);
  void update(void);
private:
  void init(uint8_t anodes[],uint8_t cath_a, uint8_t cath_b, uint8_t cath_c, uint8_t cath_d);
};

#endif
