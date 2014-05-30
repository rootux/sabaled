#include "PulseEffect.h"
#include "Arduino.h"

PulseEffect::PulseEffect(Section *sections, int sectionsStart, int sectionsEnd, int* globalSpeedFactor) : 
	BaseEffect(sections, sectionsStart, sectionsEnd, globalSpeedFactor) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	this->pulse_step_forward = 10;
	this->pulse_step_backwrads = 10;
	this->pulse_max = 200;
	this->pulse_index = 0;
	this->pulse_current_level = 0;
}

PulseEffect::~PulseEffect() {

}

void PulseEffect::setSourceColor(uint32_t color) {

}

/*Mapping is
* 0-3 RIGHT TORSO HAND STICK
* 4-7 CHEST HEAD BEARD
* 8-9 LEFT TORSO HAND
* 10 RIGHT LEG
* 11 LEFT LEG
*/
void PulseEffect::tick(void) {
	// callcounter++;
	// if (callcounter % (*globalSpeedFactor) != 0)
	//  	return;
	 
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	
	if (pulse_current_level < pulse_max) {
		pulse_current_level += pulse_step_forward;
	}

	// go up
	int temp_level = pulse_current_level;
	int temp_index = pulse_index;
	// Forward leds light up
	while (temp_level > 0) {
		strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
		temp_index++;
		temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step_forward);
	}

	int lastBackwardDimIndex = 0;
	// Only when we reach the maximum level (graceful startup) we start turning off
	// or dimming the leds behind us
	if (pulse_current_level >= pulse_max) {
		lastBackwardDimIndex = this->pulseBackwardDim();
	}

	strip->show();

	// Only when we reach max level, start advancing.
	// This creates the effect of the pulse taking shape at the beginning of the strip instead of starting in the middle of it.
	if (pulse_current_level >= pulse_max) {
		pulse_index++;
	}

	// We know this last pixel was dimmed all the way down.
	// If this pixel dimmed all the way down is the last pixel
	// in the strip - we can start over.
	if (lastBackwardDimIndex == strip->numPixels()) {
		pulse_current_level = 0;
		pulse_index = 0;
	}
}

int PulseEffect::pulseBackwardDim() {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	
	int temp_index = pulse_index - 1;
	int temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backwrads);
	// Backward leds shut down
	while (temp_index >= 0 && temp_level >= 0) {
		strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
		temp_index--;
		temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backwrads);
	}

	return temp_index;
}
