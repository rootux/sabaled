#include "ColorWipeEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

ColorWipeEffect::ColorWipeEffect(Section *sections, int sectionsStart, int sectionsEnd,int* globalSpeedFactor) : 
	BaseEffect(sections, sectionsStart, sectionsEnd, globalSpeedFactor) {
		Section *sect = &sections[sectionsStart];
		Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	setSourceColor(Adafruit_NeoPixel::Color(255, 50, 0));
	this->currentIndex = 0;
	this->currentStep = 5;
}

ColorWipeEffect::~ColorWipeEffect() {

}

void ColorWipeEffect::setSourceColor(uint32_t color) {
	this->source_color_value = color;
	this->source_color = &source_color_value;
}

void ColorWipeEffect::tick(void) {
	callcounter++;
	if (callcounter % (*globalSpeedFactor) != 0)
	 	return;
	 
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	// Fill the dots one after the other with a color
	int tempIndex = currentIndex;
	
	uint32_t tempColorValue = (uint32_t)*source_color;

    strip->setBrightness(tempIndex);
    strip->setPixelColor(tempIndex, *source_color);
    strip->show();

	currentIndex++;

	// Touched the end of the strip - go back to the start
	if (tempIndex == strip->numPixels()) {
		currentIndex = 0;
	}
}

