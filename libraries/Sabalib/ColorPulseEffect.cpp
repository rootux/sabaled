#include "ColorPulseEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

ColorPulseEffect::ColorPulseEffect(Section *sections) : BaseEffect(sections) {
	sections->strip->begin();
	sections->strip->show();

	sections->strip->setPixelColor(2, sections->strip->Color(255, 0, 0));
	sections->strip->show();

	this->source_color_value = sections->strip->Color(0, 0, 100);
	this->source_color = &source_color_value;
	this->dest_color_value = sections->strip->Color(255, 0, 0);
	this->dest_color = &dest_color_value;
	this->colorPulseCurrentIndex = 0;
	this->colorPulseTransitionStep = 5;
}

ColorPulseEffect::~ColorPulseEffect() {

}

void ColorPulseEffect::tick(void) {

	Adafruit_NeoPixel *strip = sections->strip;

	// We begin the transition from the current index going backwards
	// because the source color is where we come from and destination color
	// is where we're heading. To see the transitin come out smoothly on the
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

