#ifndef SABALIB_H
#define SABALIB_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Section
{
  public:
    Section(int startIndex, int endIndex, Adafruit_NeoPixel *strip);
    Section();
    ~Section();
    void begin(void);
    void show(void);
    Adafruit_NeoPixel *strip;
  protected:
    int startIndex;
    int endIndex;
};

class PartBase
{
  public:
    PartBase(Section *sections, int numberOfSections);
    ~PartBase();
    virtual void tick(void);
  protected:
    Section *sections;
    int numberOfSections;
};

class BogusPart: public PartBase
{
  protected:
    int blink_direction;
    int pulse_step;
    int pulse_max;
    int pulse_index;
    int pulse_current_level;
  public:
    BogusPart(Section *sections, int numberOfSections);
    ~BogusPart();
    void tick(void);
};

#endif // SABALIB_H
