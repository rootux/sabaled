#ifndef PULSEEFFECT_H
#define PULSEEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"
class PulseEffect : public BaseEffect
{
  public:
    PulseEffect(Section* section);
    ~PulseEffect();
    void tick(void);
};

#endif // PULSEEFFECT_H