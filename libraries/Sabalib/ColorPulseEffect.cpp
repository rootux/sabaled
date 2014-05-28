#include "ColorPulseEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

ColorPulseEffect::ColorPulseEffect(Section *sections, 
	int sectionsStart, int sectionsEnd, int* globalSpeedFactor) : 
	BaseEffect(sections, sectionsStart, sectionsEnd, globalSpeedFactor) {
		Section *sect = &sections[sectionsStart];
		Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	setSourceColor(Adafruit_NeoPixel::Color(255, 50, 0));
	setDestColor(Adafruit_NeoPixel::Color(191, 0, 255));
	this->colorPulseCurrentIndex = 0;
	this->colorPulseTransitionStep = 5;
	this->globalSpeedFactor = globalSpeedFactor;
	//this->callCounter = 0;
}

ColorPulseEffect::~ColorPulseEffect() {

}

void ColorPulseEffect::setSourceColor(uint32_t color) {
	this->source_color_value = color;
	this->source_color = &source_color_value;
}

void ColorPulseEffect::setDestColor(uint32_t color) {
	this->dest_color_value = color;
	this->dest_color = &dest_color_value;
}

void ColorPulseEffect::tick(void) {
	callcounter++;
	if (callcounter % (*globalSpeedFactor) != 0)
	 	return;
	 
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	// We begin the transition from the current index going backwards
	// because the source color is where we come from and destination color
	// is where we're heading. To see the transition come out smoothly on the
	// strip we should start from the source, and change the color to be the
	// most distinct (equal to destination color) at the farthest point
	int tempIndex = colorPulseCurrentIndex;
	
	uint32_t tempColorValue = (uint32_t)*source_color;
	uint32_t *tempColor = &tempColorValue;
	bool innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, dest_color, 40, 40);
	}

	// We have now reached the furthest point, i.e tempColor is the desination color.
	// Now we want to gradually go back to the original color (source_color). The
	// following code does just that
	innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, source_color, colorPulseTransitionStep, colorPulseTransitionStep);
	}

	strip->setPixelColor(tempIndex, *tempColor);

	strip->show();

	colorPulseCurrentIndex++;

	// Touched the end of the strip - go back to the start
	if (tempIndex == strip->numPixels()) {
		colorPulseCurrentIndex = 0;
	}
}

