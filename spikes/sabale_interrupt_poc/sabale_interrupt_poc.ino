#include <TimerThree.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

uint32_t RED_MASK = 0xFF0000;
uint16_t GREEN_MASK = 0xFF00;
uint16_t BLUE_MASK = 0xFF;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(240, PIN, NEO_GRB + NEO_KHZ800);


const int led = LED_BUILTIN;  // the pin with a LED

void setup(void) {
	strip.begin();
	for (int i = 0; i < strip.numPixels(); i++)
		strip.setPixelColor(i, strip.Color(0, 0, 0));
	strip.show();
	pinMode(led, OUTPUT);
	Timer3.initialize(100000);
//	Timer3.attachInterrupt(pulse); // blinkLED to run every 0.01 second
//	Timer3.attachInterrupt(colorPulse);
	Serial.begin(9600);
}


#ifdef TEST_TRANSITION_STEP
		if (current_color != dest_color) {
			bool equal = transitionStep(current_color, dest_color, colorPulseTransitionStep, colorPulseTransitionStep);
			if (equal) {
				Serial.println("EQUAL");
			}
		}

		for (int j = 0; j < strip.numPixels(); ++j) {
			strip.setPixelColor(j, *current_color);
		}

		strip.show();
#endif

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

uint32_t source_color_value = strip.Color(255, 0, 0);
uint32_t *source_color = &source_color_value;
uint32_t *current_color = source_color;
uint32_t dest_color_value = strip.Color(0, 0, 255);
uint32_t *dest_color = &dest_color_value;
bool isAtEnd = false;
int colorPulseCurrentIndex = 0;
int colorPulseTransitionStep = 10;

boolean transitionStep(uint32_t *src_color, uint32_t *dst_color, int forwardStep, int backwardsStep);

/* Color pulse
*/

int call_counter = 0;

bool firstRun = true;

void colorPulse(void) {


//	if ((uint32_t)*current_color == (uint32_t)*dest_color) {
//		colorPulseCurrentIndex++;
//	}
}

/* Pulse */
/*
  For pulse effect: step_fowrad and step_backwards must be equal
  For firework effect: forward step must be a lot bigger (~x8) than backwards step
  For reverse-pulse: step_backwrds must be a lot bigger (~x8) than forward step
*/
int pulse_step_forward = 17;
int pulse_step_backwrads = 17;
int pulse_max = 255;
int pulse_index = 0;
int pulse_current_level = 0;

void pulseBackwardDim(int temp_level, int temp_index);

void pulse(void) {
	if (pulse_current_level < pulse_max) {
		pulse_current_level += pulse_step_forward;
	}

	// go up
	int temp_level = pulse_current_level;
	int temp_index = pulse_index;
	// Forward leds light up
	while (temp_level > 0) {
		strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
		temp_index++;
		temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step_forward);
	}

	int lastBackwardDimIndex = 0;
	// Only when we reach the maximum level (graceful startup) we start turning off
	// or dimming the leds behind us
	if (pulse_current_level == pulse_max) {
		lastBackwardDimIndex = pulseBackwardDim();
	}

	strip.show();

	// Only when we reach max level, start advancing.
	// This creates the effect of the pulse taking shape at the beginning of the strip instead of starting in the middle of it.
	if (pulse_current_level == pulse_max) {
		pulse_index++;
	}

	// We know this last pixel was dimmed all the way down.
	// If this pixel dimmed all the way down is the last pixel
	// in the strip - we can start over.
	if (lastBackwardDimIndex == strip.numPixels()) {
		pulse_current_level = 0;
		pulse_index = 0;
	}
}

/*
  pulseBackwardDim - Dims the pixels behind a specific point.
  works with pulse_index and pulse_current level to determine where it should dim from
  returns: the index of the last pixel with any level
*/
int pulseBackwardDim() {
	int temp_index = pulse_index - 1;
	int temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backwrads);
	// Backward leds shut down
	while (temp_index >= 0 && temp_level >= 0) {
		strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
		temp_index--;
		temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backwrads);
	}

	return temp_index;
}


uint8_t nextTransitionValue(uint8_t source, uint8_t destination, int forwardStep, int backwardsStep);

/*
	transitionStep
	Moves from src_color to dst_color in steps defined by forwardStep and backwardsStep
	*/
boolean transitionStep(uint32_t *src_color, uint32_t *dst_color, int forwardStep, int backwardsStep) {
	uint8_t r = (*src_color & RED_MASK) >> 16;
	uint8_t g = (*src_color & GREEN_MASK) >> 8;
	uint8_t b = (*src_color & BLUE_MASK);

	uint8_t rdest = (*dst_color & RED_MASK) >> 16;
	uint8_t gdest = (*dst_color & GREEN_MASK) >> 8;
	uint8_t bdest = (*dst_color & BLUE_MASK);

	r = nextTransitionValue(r, rdest, forwardStep, backwardsStep);
	g = nextTransitionValue(g, gdest, forwardStep, backwardsStep);
	b = nextTransitionValue(b, bdest, forwardStep, backwardsStep);

	*src_color = strip.Color(r, g, b);

//	Serial.print("r = ");
//	Serial.print(r);
//	Serial.print("g = ");
//	Serial.print(g);
//	Serial.print("b = ");
//	Serial.println(b);

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

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void) {
	if (firstRun) {
		for (int i = 0; i < strip.numPixels(); ++i) {
			strip.setPixelColor(i, *source_color);
		}

		firstRun = false;
	}

	int tempIndex = colorPulseCurrentIndex;
	uint32_t tempColorValue = *source_color;
	uint32_t *tempColor = &tempColorValue;
	bool innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip.setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = transitionStep(tempColor, dest_color, colorPulseTransitionStep, colorPulseTransitionStep);
	}

	Serial.println(tempIndex);
	innerTransitionComplete = false;
	while (!innerTransitionComplete && tempIndex >= 0) {
		strip.setPixelColor(tempIndex, *tempColor);
		tempIndex--;
		innerTransitionComplete = transitionStep(tempColor, source_color, colorPulseTransitionStep, colorPulseTransitionStep);
	}

	strip.show();
	delay(50);

	colorPulseCurrentIndex++;
}















































