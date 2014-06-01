#ifndef GLOWEFFECT_H
#define GLOWEFFECT_H

#include "Arduino.h"
#include "BaseEffect.h"

class GlowEffect : public BaseEffect {
public:
	GlowEffect(Section *sections,
			int sectionsStart, int sectionsEnd);

	~GlowEffect();

	void tick(void);
private:
	int direction;
	int transitionStep;
	int transitionStepBackwards;
	uint32_t currentColorValue;
	uint32_t *currentColor;
	uint32_t currentIndex;
};

#endif // GLOWEFFECT_H