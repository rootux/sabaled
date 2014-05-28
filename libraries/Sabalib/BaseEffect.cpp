#include "BaseEffect.h"

BaseEffect::BaseEffect(Section *sections, int sectionsStart, int sectionsEnd,int* globalSpeedFactor) {
	this->sections = sections;
	this->sectionsStart = sectionsStart;
	this->sectionsEnd = sectionsEnd;
	this->globalSpeedFactor = globalSpeedFactor;
}

BaseEffect::~BaseEffect() {

}