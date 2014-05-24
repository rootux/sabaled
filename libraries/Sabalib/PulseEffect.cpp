#include "PulseEffect.h"

PulseEffect::PulseEffect(Section* sections): BaseEffect(sections) {
}

PulseEffect::~PulseEffect() {

}

void PulseEffect::tick(void) { 
	Serial.println("TICK!");
}