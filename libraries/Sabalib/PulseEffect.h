#ifndef PULSEEFFECT_H
#define PULSEEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class PulseEffect : public BaseEffect {
public:
	PulseEffect(Section *section,int sectionsStart, int sectionsEnd, int* globalSpeedFactor);

	~PulseEffect();

	void tick(void);
	void setSourceColor(uint32_t color);
	
private:
	int pulse_step_forward;
	int pulse_step_backwrads;
	int pulse_max;
	int pulse_index;
	int pulse_current_level;

	int pulseBackwardDim();
};

#endif // PULSEEFFECT_H