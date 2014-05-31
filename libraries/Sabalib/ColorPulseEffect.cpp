#include "ColorPulseEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

#define DIRECTION_FORWARD 0
#define DIRECTION_BACKWARDS 1

ColorPulseEffect::ColorPulseEffect(Section *sections,
		int sectionsStart, int sectionsEnd, int direction) :
		BaseEffect(sections, sectionsStart, sectionsEnd, 0) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

//	this->slowDownFactor = 1;
//	// Back + head + beard || heart
//	if (sectionsStart == 4 || sectionsStart == 12) {
//		this->slowDownFactor =  6;
//	}

	if (direction == DIRECTION_BACKWARDS) {
		this->colorPulseCurrentIndex = strip->numPixels();
	} else {
		this->colorPulseCurrentIndex = 0;
	}

	this->direction = direction;

	if (this->isHighRes) {
		this->transitionStep = 4;
	} else {
		this->transitionStep = 40;
	}

	this->callCounter = 0;
}

ColorPulseEffect::~ColorPulseEffect() {

}

void ColorPulseEffect::tick(void) {
	if (direction == DIRECTION_BACKWARDS) {
		tickBackwards();
	} else {
		tickForward();
	}
}

void ColorPulseEffect::tickBackwards() {

//	callcounter++;
//	if (callcounter % (*SabaleUtils::globalSpeedFactor / this->slowDownFactor) != 0)
//	  	return;

	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	// We begin the transition from the current index going backwards
	// because the source color is where we come from and destination color
	// is where we're heading. To see the transition come out smoothly on the
	// strip we should start from the source, and change the color to be the
	// most distinct (equal to destination color) at the farthest point
	int tempIndex = colorPulseCurrentIndex;

	uint32_t tempColorValue = (uint32_t) * SabaleUtils::globalSourceColor;
	uint32_t *tempColor = &tempColorValue;
	bool innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex <= (int)strip->numPixels()) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex++;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, SabaleUtils::globalDestColor, transitionStep, transitionStep);
	}

	// We have now reached the furthest point, i.e tempColor is the desination color.
	// Now we want to gradually go back to the original color (source_color). The
	// following code does just that
	innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex <= (int)strip->numPixels()) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex++;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, SabaleUtils::globalSourceColor, transitionStep, transitionStep);
	}

	strip->setPixelColor(tempIndex, *tempColor);

	strip->show();

	colorPulseCurrentIndex--;

	// Touched the end of the strip - go back to the start
	if (tempIndex <= 0) {
		colorPulseCurrentIndex = strip->numPixels();
	}
}

void ColorPulseEffect::tickForward() {

//	callcounter++;
//	if (callcounter % (*SabaleUtils::globalSpeedFactor / this->slowDownFactor) != 0)
//	  	return;

	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	// We begin the transition from the current index going backwards
	// because the source color is where we come from and destination color
	// is where we're heading. To see the transition come out smoothly on the
	// strip we should start from the source, and change the color to be the
	// most distinct (equal to destination color) at the farthest point
	int tempIndex = colorPulseCurrentIndex;

	uint32_t tempColorValue = (uint32_t) * SabaleUtils::globalSourceColor;
	uint32_t *tempColor = &tempColorValue;
	bool innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, SabaleUtils::globalDestColor, transitionStep, transitionStep);
	}

	// We have now reached the furthest point, i.e tempColor is the desination color.
	// Now we want to gradually go back to the original color (source_color). The
	// following code does just that
	innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip->setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = SabaleUtils::transitionStep(tempColor, SabaleUtils::globalSourceColor, transitionStep, transitionStep);
	}

	strip->setPixelColor(tempIndex, *tempColor);

	strip->show();

	colorPulseCurrentIndex++;

	// Touched the end of the strip - go back to the start
	if (tempIndex == strip->numPixels()) {
		colorPulseCurrentIndex = 0;
	}
}
