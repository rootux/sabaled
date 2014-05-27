#include "HeartBeatEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

HeartBeatEffect::HeartBeatEffect(Section *sections, int sectionsStart, int sectionsEnd) : 
	BaseEffect(sections, sectionsStart, sectionsEnd) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	setSourceColor(Adafruit_NeoPixel::Color(255, 50, 0));
	setDestColor(Adafruit_NeoPixel::Color(191, 0, 255));
	this->colorPulseCurrentIndex = 0;
	this->colorPulseTransitionStep = 5;
	this->currentBrightness = strip->numPixels();
}

HeartBeatEffect::~HeartBeatEffect() {

}

void HeartBeatEffect::setSourceColor(uint32_t color) {
	this->source_color_value = color;
	this->source_color = &source_color_value;
}

void HeartBeatEffect::setDestColor(uint32_t color) {
	this->dest_color_value = color;
	this->dest_color = &dest_color_value;
}

void HeartBeatEffect::tick(void) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	int tempIndex = colorPulseCurrentIndex;
	uint32_t tempColorValue = (uint32_t)*source_color;
	uint32_t *tempColor = &tempColorValue;

	//Dim it
	if(currentBrightness > 0) {
      	strip->setPixelColor(tempIndex, tempColorValue);
      	strip->setBrightness(currentBrightness);
      	strip->show();
      	tempIndex++;
      	currentBrightness--;
      	delay(50);
      	return;
  	}

  	tempIndex--;


  	delay(1000);
  	  if(tempIndex > 0) {
  	  	strip->setPixelColor(tempIndex, tempColorValue);
	    strip->setBrightness(tempIndex);
	    strip->show();
	  }

	  currentBrightness = strip->numPixels();

	colorPulseCurrentIndex++;

	// Touched the end of the strip - go back to the start
	if (tempIndex == strip->numPixels()) {
		colorPulseCurrentIndex = 0;
	}
}

