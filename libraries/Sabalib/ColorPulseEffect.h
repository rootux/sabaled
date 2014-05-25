#ifndef COLORPULSEFFECT_H
#define COLORPULSEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class ColorPulseEffect : public BaseEffect {
public:
	ColorPulseEffect(Section *section);

	~ColorPulseEffect();

	void tick(void);

private:
	uint32_t source_color_value;
	uint32_t dest_color_value;
	uint32_t *source_color;
	uint32_t *dest_color;
	int colorPulseCurrentIndex;
	int colorPulseTransitionStep;
};

#endif // COLORPULSEFFECT_H