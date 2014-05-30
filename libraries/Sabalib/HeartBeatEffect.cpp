#include "HeartBeatEffect.h"
#include "Arduino.h"
#include "Sabalib.h"

HeartBeatEffect::HeartBeatEffect(Section *sections, int sectionsStart, int sectionsEnd,int* globalSpeedFactor) : 
	BaseEffect(sections, sectionsStart, sectionsEnd,globalSpeedFactor) {
	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;
	strip->begin();
	strip->show();

	setSourceColor(Adafruit_NeoPixel::Color(255, 50, 0));
	this->currentIndex = 0;
	this->currentStep = 5;
	this->currentBrightness = strip->numPixels();
}

HeartBeatEffect::~HeartBeatEffect() {

}

void HeartBeatEffect::setSourceColor(uint32_t color) {
	this->source_color_value = color;
	this->source_color = &source_color_value;
}

void HeartBeatEffect::tick(void) {
	// callcounter++;
	// if (callcounter % (*globalSpeedFactor) != 0)
	//  	return;

	Section *sect = &sections[sectionsStart];
	Adafruit_NeoPixel *strip = sect->strip;

	int tempIndex = currentIndex;
	uint32_t tempColorValue = (uint32_t)*source_color;
	uint32_t *tempColor = &tempColorValue;

	
	//Dim it
	if(currentBrightness > 0) {
      	strip->setPixelColor(tempIndex, tempColorValue);
      	strip->setBrightness(currentBrightness);
      	strip->show();
      	currentIndex++;
      	currentBrightness--;
      	// delay(50);
      	return;
  	}


  	  if(tempIndex > 0) {
  	  	strip->setPixelColor(tempIndex, tempColorValue);
	    strip->setBrightness(currentBrightness);
	    strip->show();
	    currentIndex--;
	    return;
	  }

	currentBrightness = strip->numPixels();

	currentIndex++;

	// Touched the end of the strip - go back to the start
	 if (tempIndex == strip->numPixels()) {
	 	currentIndex = 0;
	 }
}

