#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Arduino.h"
#include "Sabalib.h"

class BaseEffect {
public:
	BaseEffect(Section *sections, int sectionsStart, int sectionsEnd, int* globalSpeedFactor);

	~BaseEffect();

	void tick(void);

protected:
	Section *sections;
	int sectionsStart;
	int sectionsEnd;
	int callCounter;
	bool isHighRes;
};

#endif // BASEEFFECT_H