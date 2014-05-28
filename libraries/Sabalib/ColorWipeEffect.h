#ifndef COLORWIPEEFFECT_H
#define COLORWIPEEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class ColorWipeEffect : public BaseEffect {
public:
	ColorWipeEffect(Section *section, int sectionsStart, int sectionsEnd);

	~ColorWipeEffect();

	void tick(void);

	void setSourceColor(uint32_t color);
private:
	uint32_t source_color_value;
	uint32_t *source_color;
	int currentIndex;
	int currentStep;
};

#endif // COLORWIPEEFFECT_H