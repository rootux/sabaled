#ifndef COLORPULSEFFECT_H
#define COLORPULSEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class ColorPulseEffect : public BaseEffect {
public:
	ColorPulseEffect(Section *sections,
			int sectionsStart, int sectionsEnd, int direction);

	~ColorPulseEffect();

	void tick(void);

	void setSourceColor(uint32_t color);
	void setDestColor(uint32_t color);
private:
	uint32_t source_color_value;
	uint32_t dest_color_value;
	uint32_t *source_color;
	uint32_t *dest_color;
	int callCounter;
	int slowDownFactor;
	int direction;
	int colorPulseCurrentIndex;
	int transitionStep;

	void tickBackwards();

	void tickForward();
};

#endif // COLORPULSEFFECT_H