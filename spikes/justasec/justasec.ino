#include <TimerThree.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.

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
	strip.show();
	pinMode(led, OUTPUT);
	Timer3.initialize(10000);
	Timer3.attachInterrupt(pulse); // blinkLED to run every 0.01 second
	Serial.begin(9600);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

/* Firework */
void firework(void) {
}


/* Pulse */
/*
  For pulse effect: step_fowrad and step_backwards must be equal
  For firework effect: forward step must be a lot bigger (~x8) than backwards step
  For reverse-pulse: step_backwrds must be a lot bigger (~x8) than forward step
*/
int pulse_step_forward = 40;
int pulse_step_backward = 5;
int pulse_max = 120;
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

	// Only when we reach the maximum level (graceful startup) we start turning off
	// or dimming the leds behind us
	if (pulse_current_level == pulse_max) {
		pulseBackwardDim();
	}

	strip.show();

	// Only when we reach max level, start advancing.
	// This creates the effect of the pulse taking shape at the beginning of the strip instead of starting in the middle of it.
	if (pulse_current_level == pulse_max) {
		pulse_index++;
	}

	if (pulse_index == strip.numPixels()) {
		// Reaches the end of the strip, end the pulse gracefuly in a manner
		// similar to the one in which we begin i.e decade the pulse slowly into the
		// end of the strip instead of turning off multiple leds at once.
		while (pulse_current_level > 0) {
			pulseBackwardDim();

			pulse_current_level -= pulse_step_backward;
		}

		pulse_current_level = 0;
		pulse_index = 0;
	}
}

void pulseBackwardDim() {
	int temp_index = pulse_index - 1;
	int temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backward);
	// Backward leds shut down
	while (temp_index >= 0 && temp_level >= 0) {
		strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
		temp_index--;
		temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step_backward);
	}
}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void) {
//	unsigned long blinkCopy;  // holds a copy of the blinkCount
//
//	// to read a variable which the interrupt code writes, we
//	// must temporarily disable interrupts, to be sure it will
//	// not change while we are reading.  To minimize the time
//	// with interrupts off, just quickly make a copy, and then
//	// use the copy while allowing the interrupt to keep working.
//	noInterrupts();
//	blinkCopy = blinkCount;
//	interrupts();
//
//	delay(100);
}
