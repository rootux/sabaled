#ifndef COLORPULSEFFECT_H
#define COLORPULSEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class ColorPulseEffect : public BaseEffect {
public:
	ColorPulseEffect(Section *section, int sectionsStart, int sectionsEnd, int* globalSpeedFactor);

	~ColorPulseEffect();

	void tick(void);

	void setSourceColor(uint32_t color);
	void setDestColor(uint32_t color);
private:
	uint32_t source_color_value;
	uint32_t dest_color_value;
	uint32_t *source_color;
	uint32_t *dest_color;
	int colorPulseCurrentIndex;
	int colorPulseTransitionStep;
};

#endif // COLORPULSEFFECT_H