#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class BaseEffect
{
  public:
    BaseEffect();
    ~BaseEffect();
    virtual void tick(void);
};

#endif // BASEEFFECT_H