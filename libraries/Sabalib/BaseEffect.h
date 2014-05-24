#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Arduino.h"
#include "Sabalib.h"

class BaseEffect
{
  public:
    BaseEffect(Section* sections);
    ~BaseEffect();
    void tick(void);
  protected:
  	Section* sections;
};

#endif // BASEEFFECT_H