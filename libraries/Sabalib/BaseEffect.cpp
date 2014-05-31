#include "BaseEffect.h"

BaseEffect::BaseEffect(Section *sections, int sectionsStart, int sectionsEnd,int* globalSpeedFactor) {
	this->sections = sections;
	this->sectionsStart = sectionsStart;
	this->sectionsEnd = sectionsEnd;

	this->isHighRes = false;

	// Back + head + beard || heart
	if (sectionsStart == 4 || sectionsStart == 12) {
		this->isHighRes = true;
	}
}

BaseEffect::~BaseEffect() {

}