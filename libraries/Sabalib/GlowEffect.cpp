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

	currentColorValue = (uint32_t) * SabaleUtils::globalSourceColor;
	currentColor = &currentColorValue;
	currentIndex = 0;
}


GlowEffect::~GlowEffect() {

}

void GlowEffect::tick(void) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	boolean transitionComplete =
			SabaleUtils::transitionStep(currentColor, SabaleUtils::globalDestColor, transitionStep, transitionStep);

	if (transitionComplete) {
		*currentColor = (uint32_t) * SabaleUtils::globalSourceColor;
	}

	strip->setPixelColor(currentIndex, *currentColor);
	strip->show();

	currentIndex++;
	if(currentIndex == strip->numPixels()) {
		currentIndex = 0;
	}
}
