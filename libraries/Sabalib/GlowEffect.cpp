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

	currentColorValue = (uint32_t) * SabaleUtils::heartSourceColor;
	currentColor = &currentColorValue;
	currentIndex = 0;
	transitionStep = 4;
}


GlowEffect::~GlowEffect() {

}

void GlowEffect::tick(void) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	boolean transitionComplete =
			SabaleUtils::transitionStep(currentColor, SabaleUtils::heartDestColor, transitionStep, transitionStep);


	Serial.println(*currentColor);
	if (transitionComplete) {
		*currentColor = (uint32_t) * SabaleUtils::heartSourceColor;
	}
	
	for(int i=0;i<strip->numPixels();i++) {
	  strip->setPixelColor(i, *currentColor);
	}
	strip->show();

}
