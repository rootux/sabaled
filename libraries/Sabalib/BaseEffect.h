#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "Arduino.h"
#include "Sabalib.h"

class BaseEffect {
public:
	BaseEffect(Section *sections, int sectionsStart, int sectionsEnd);

	~BaseEffect();

	void tick(void);

protected:
	Section *sections;
	int sectionsStart;
	int sectionsEnd;
};

#endif // BASEEFFECT_H