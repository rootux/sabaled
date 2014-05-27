#include "BaseEffect.h"

BaseEffect::BaseEffect(Section *sections, int sectionsStart, int sectionsEnd) {
	this->sections = sections;
	this->sectionsStart = sectionsStart;
	this->sectionsEnd = sectionsEnd;
}

BaseEffect::~BaseEffect() {

}