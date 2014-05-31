#ifndef SABALIB_H
#define SABALIB_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class SabaleUtils {
public:
	static boolean transitionStep(uint32_t *src_color, uint32_t *dst_color, int forwardStep, int backwardsStep);

	static int globalSpeedFactorValue;
	static int* globalSpeedFactor;
	static uint32_t globalSourceColorValue;
	static uint32_t globalDestColorValue;
	static uint32_t heartSourceColorValue;
	static uint32_t heartDestColorValue;
	static uint32_t* globalSourceColor;
	static uint32_t* globalDestColor;
	static uint32_t* heartSourceColor;
	static uint32_t* heartDestColor;
};

// static uint32_t SabaleUtils::globalSourceColorValue;
// static uint32_t SabaleUtils::globalDestColorValue;
// static uint32_t SabaleUtils::heartSourceColorValue;
// static uint32_t SabaleUtils::heartDestColorValue;
// static uint32_t* SabaleUtils::globalSourceColor;
// static uint32_t* SabaleUtils::globalDestColor;
// static uint32_t* SabaleUtils::heartSourceColor;
// static uint32_t* SabaleUtils::heartDestColor;

class Section {
public:
	Section(int startIndex, int endIndex, Adafruit_NeoPixel *strip);

	Section();

	~Section();

	void begin(void);

	void show(void);

	Adafruit_NeoPixel *strip;
protected:
	int startIndex;
	int endIndex;
};

class PartBase {
public:
	PartBase(Section *sections, int numberOfSections);

	~PartBase();

	virtual void tick(void);

protected:
	Section *sections;
	int numberOfSections;
};

class BogusPart : public PartBase {
protected:
	int blink_direction;
	int pulse_step;
	int pulse_max;
	int pulse_index;
	int pulse_current_level;
public:
	BogusPart(Section *sections, int numberOfSections);

	~BogusPart();

	void tick(void);
};

#endif // SABALIB_H
