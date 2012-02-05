#ifndef	WATT_SAMPLE
#define	WATT_SAMPLE

typedef struct {
  float	vrms;
  float	irms;
  float	watt;
  unsigned long count;
} 
WattSample;

static void clearSample(WattSample &sample)
{
  sample.vrms = sample.irms = sample.watt = sample.count = 0;
}

static void setSample(WattSample &sample, float v, float a, float w)
{
  sample.vrms = v;
  sample.irms = a;
  sample.watt = w;
  sample.count = 1;
}

static void addSample(WattSample &a, WattSample &b)
{
  a.vrms += b.vrms;
  a.irms += b.irms;
  a.watt += b.watt;
  a.count += b.count;
}

static float meanVrms(WattSample &sample)
{
  return sample.vrms / sample.count;
}

static float meanIrms(WattSample &sample)
{
  return sample.irms / sample.count;
}

static float meanWatt(WattSample &sample)
{
  return sample.watt / sample.count;
}

#endif

