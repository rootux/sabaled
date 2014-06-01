#include "GlowEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

#define DIRECTION_FORWARD 0
#define DIRECTION_BACKWARDS 1

GlowEffect::GlowEffect(Section *sections, int sectionsStart, int sectionsEnd)
:BaseEffect(sections, sectionsStart, sectionsEnd, 0){
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	currentColorValue = (uint32_t)* SabaleUtils::heartSourceColor;
	currentColor = &currentColorValue;
	transitionStep = 30;
	transitionStepBackwards = 4;
	direction = DIRECTION_FORWARD;
}


GlowEffect::~GlowEffect() {

}

void GlowEffect::tick(void) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	boolean transitionComplete;
	if (direction == DIRECTION_FORWARD){
		transitionComplete =
			SabaleUtils::transitionStep(currentColor, SabaleUtils::heartDestColor, transitionStep, transitionStep);
	}
	else if (direction == DIRECTION_BACKWARDS) {
		transitionComplete =
			SabaleUtils::transitionStep(currentColor, SabaleUtils::heartSourceColor, transitionStepBackwards, transitionStepBackwards);
	}

	if (transitionComplete) {
		if (direction == DIRECTION_FORWARD) {
			direction = DIRECTION_BACKWARDS;
		}
		else if (direction == DIRECTION_BACKWARDS) {
			direction = DIRECTION_FORWARD;
		}
	}

	for (int i = 0; i < strip->numPixels(); i++) {
		strip->setPixelColor(i, *currentColor);
	}

	strip->show();
}
