#ifndef HEARTBEATEFFECT_H
#define HEARTBEATEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class HeartBeatEffect : public BaseEffect {
public:
	HeartBeatEffect(Section *section, int sectionsStart, int sectionsEnd, int* globalSpeedFactor);

	~HeartBeatEffect();

	void tick(void);

	void setSourceColor(uint32_t color);
	void setDestColor(uint32_t color);
private:
	uint32_t source_color_value;
	uint32_t *source_color;
	int currentIndex;
	int currentStep;
	int currentBrightness;
};

#endif // HEARTBEATEFFECT_H