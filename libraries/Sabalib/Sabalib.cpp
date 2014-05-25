#include "Sabalib.h"
#include "Adafruit_NeoPixel.h"

uint32_t RED_MASK = 0xFF0000;
uint16_t GREEN_MASK = 0xFF00;
uint16_t BLUE_MASK = 0xFF;

uint8_t nextTransitionValue(uint8_t source, uint8_t destination, int forwardStep, int backwardsStep);

boolean SabaleUtils::transitionStep(uint32_t *src_color, uint32_t *dst_color, int forwardStep, int backwardsStep) {
	uint8_t r = (*src_color & RED_MASK) >> 16;
	uint8_t g = (*src_color & GREEN_MASK) >> 8;
	uint8_t b = (*src_color & BLUE_MASK);

	uint8_t rdest = (*dst_color & RED_MASK) >> 16;
	uint8_t gdest = (*dst_color & GREEN_MASK) >> 8;
	uint8_t bdest = (*dst_color & BLUE_MASK);

	r = nextTransitionValue(r, rdest, forwardStep, backwardsStep);
	g = nextTransitionValue(g, gdest, forwardStep, backwardsStep);
	b = nextTransitionValue(b, bdest, forwardStep, backwardsStep);

	*src_color = Adafruit_NeoPixel::Color(r, g, b);

	return (r == rdest) && (g == gdest) && (b == bdest);
}

uint8_t nextTransitionValue(uint8_t source, uint8_t destination, int forwardStep, int backwardsStep) {
	int result;
	if (source < destination) {
		result = source + forwardStep;
		if (result > destination) {
			// went too far
			result = destination;
		}
	} else if (source > destination) {
		result = source - backwardsStep;
		if (result < destination) {
			// went too far
			result = destination;
		}
	} else {
		result = destination;
	}

	return (uint8_t) result;
}

Section::Section() {

}

Section::Section(int startIndex, int endIndex, Adafruit_NeoPixel *strip) {
	this->strip = strip;
}

void Section::begin(void) {
	Serial.println("Init section");
	strip->begin();
}

void Section::show(void) {
	strip->show();
}

Section::~Section() {

}

PartBase::PartBase(Section *sections, int numberOfSections) :
		sections(sections), numberOfSections(numberOfSections) {

}

PartBase::~PartBase() {

}

void PartBase::tick() {

}

BogusPart::BogusPart(Section *sections, int numberOfSections) :
		PartBase(sections, numberOfSections) {
	this->blink_direction = 1;
	this->pulse_step = 10;
	this->pulse_max = 60;
	this->pulse_index = 0;
	this->pulse_current_level = 0;
}


BogusPart::~BogusPart() {

}

void BogusPart::tick() {
	Adafruit_NeoPixel *strip = sections->strip;
	// uint8_t* pixels = strip.getPixels();
	// Serial.println('Pixels array:');
	// for(int i=0;i<strip.numPixels();i++) {
	// 	Serial.print(pixels[i]);
	// }
	if (pulse_current_level < pulse_max) {
		pulse_current_level += pulse_step;
	}

	// go up
	int temp_level = pulse_current_level;
	int temp_index = pulse_index;
	while (temp_level > 0) {
		Serial.println(temp_index);
		strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
		temp_index++;
		temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step);
	}

	if (pulse_current_level == pulse_max) {
		temp_index = pulse_index - 1;
		temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step);
		while (temp_index >= 0 && temp_level >= 0) {
			strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
			temp_index--;
			temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step);
		}
	}

	strip->show();

	if (pulse_current_level == pulse_max) {
		pulse_index++;
	}

	if (pulse_index == strip->numPixels()) {
		pulse_current_level = 0;
		pulse_index = 0;
	}
}
