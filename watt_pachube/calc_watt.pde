// define pin
const int arefPin = 0;
const int voltagePin = 4;
const int currentPin = 3;

// mains frequency
#define POWER_FREQ (60)

// number of samples per cycle
#define NUMBER_OF_SAMPLES (25)

// sample interval (in microseconds)
#define SAMPLING_PERIOD (1000000 / (POWER_FREQ * NUMBER_OF_SAMPLES))

// buffer samples
int VASamples[NUMBER_OF_SAMPLES*4];

void calcWatt (void)
{
#define kVT (86.9817579) // based on the measured coefficient
#define kCT (100.0 * 0.99 / 800.0) // R * factor / number of turns

  unsigned long t1, t2;
  int i, r, v1, a1, a2, v2;

  t1 = micros ();

  // sampling cycle of 1 AD values
  for(i=0; i<NUMBER_OF_SAMPLES; i++){

    r = analogRead(arefPin);
    v1 = analogRead(voltagePin);
    a1 = analogRead(currentPin);
    a2 = analogRead(currentPin);
    v2 = analogRead(voltagePin);

    VASamples[(i*4)+0] = v1 - r;
    VASamples[(i*4)+1] = a1 - r;
    VASamples[(i*4)+2] = a2 - r;
    VASamples[(i*4)+3] = v2 - r;

    do {
      t2 = micros();
    } 
    while((t2 - t1) < SAMPLING_PERIOD);
    t1 += SAMPLING_PERIOD;
  }

  // one cycle of voltage and current, plus the power
  Vrms = 0;
  Irms = 0;
  Watt = 0;

  for (i=0; i <NUMBER_OF_SAMPLES; i++) {
    v1 = VASamples[(i*4)+0];
    a1 = VASamples[(i*4)+1];
    a2 = VASamples[(i*4)+2];
    v2 = VASamples[(i*4)+3];

    float vv = ((((v1+v2)/2) * 5.0) / 1024) * kVT;
    float aa = ((((a1+a2)/2) * 5.0) / 1024) / kCT;

    Vrms += vv * vv;
    Irms += aa * aa;
    Watt += vv * aa;
  }

  // root mean square 2 (rms) obtained
  Vrms = sqrt(Vrms / NUMBER_OF_SAMPLES);
  Irms = sqrt(Irms / NUMBER_OF_SAMPLES);

  // find the average power
  Watt = Watt / NUMBER_OF_SAMPLES;
}

