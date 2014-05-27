#include <Adafruit_NeoPixel.h>
#include <PulseEffect.h>
#include <ColorPulseEffect.h>
#include <Sabalib.h>
#include <TimerThree.h>
#include <NewPing.h>

#define RIGHT_TORSO_HAND_STICK_PIN 3
#define CHEST_HEAD_BEARD_PIN 4
#define LEFT_TORSO_HAND_PIN 5
#define LEFT_LEG_PIN 6
#define RIGHT_LEG_PIN 7
#define HEART_PIN 8

#define BUTTONS_NUM 7
#define PARTS_NUM 6

#define COLORS_NUM 4

//Remember that there is also on/off button
#define BTN_A_PIN 21
#define BTN_B_PIN 22
#define BTN_C_PIN 23
#define BTN_D_PIN 24
#define BTN_E_PIN 25
#define BTN_F_PIN 26
#define BTN_G_PIN 27

#define SONAR_NUM 1 //number of sonars
#define MAX_DISTANCE 280 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 200 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int sonars[SONAR_NUM];     // Where the ping distances are stored.
unsigned int buttons[BUTTONS_NUM];  // Holds the buttons states
uint32_t colors[COLORS_NUM];
uint32_t currentColorIndex = 0;
uint32_t currentColor;

uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

volatile int activeEffect = 2;

//NewPing sonar[SONAR_NUM] = {     // Sensor object array.
//		NewPing(10, 11, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
////		NewPing(12, 13, MAX_DISTANCE)
//};

Section sections[13];
Adafruit_NeoPixel *strips[PARTS_NUM];
PulseEffect *pulseEffect;
ColorPulseEffect *colorPulseEffect[PARTS_NUM];

void setup() {
	Serial.begin(9600);
        initColors();
	//initSonars();
	initSections();
	Timer3.initialize(33333); // blinkLED to run every 0.01 second
	Timer3.attachInterrupt(tick);
}

void initColors() {
  colors[0] = Adafruit_NeoPixel::Color(255, 50, 0);
  colors[1] = Adafruit_NeoPixel::Color(191, 0, 255);
  colors[2] = Adafruit_NeoPixel::Color(130, 60, 110);  
  colors[3] = Adafruit_NeoPixel::Color(21, 70, 92);    
}

//void initSonars(void) {
//	// First ping starts at 75ms, gives time for the Arduino to chill before starting.
//	pingTimer[0] = millis() + 75;
//	for (uint8_t i = 1; i < SONAR_NUM; i++) {// Set the starting time for each sensor.
//		pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
//	}
//}

Adafruit_NeoPixel *testStrip = new Adafruit_NeoPixel(720, 6,NEO_GRB + NEO_KHZ800);
Section *testSection = new Section(0, 240, testStrip);
Adafruit_NeoPixel *testStrip2 = new Adafruit_NeoPixel(50, 7, NEO_GRB + NEO_KHZ800);
Section *testSection2 = new Section(0, 50, testStrip2);

void initSections(void) {
	//RIGHT TORSO HAND STICK
	strips[0] = new Adafruit_NeoPixel(200, RIGHT_TORSO_HAND_STICK_PIN, NEO_GRB + NEO_KHZ800);
	sections[0]= Section(0, 49, strips[0]);
	sections[1] = Section(50, 99, strips[0]);
	sections[2] = Section(100, 149, strips[0]);
	sections[3] = Section(150, 199, strips[0]);
        colorPulseEffect[0] = new ColorPulseEffect(sections, 0, 3);
        
	//CHEST HEAD BEARD
	strips[1] = new Adafruit_NeoPixel(960, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
	sections[4] = Section(0, 239, strips[1]);
	sections[5] = Section(240, 479, strips[1]);
	sections[6] = Section(480, 719, strips[1]);
	sections[7] = Section(720, 959, strips[1]);
        colorPulseEffect[1] = new ColorPulseEffect(sections, 4, 7);

	//LEFT TORSO HAND
	strips[2] = new Adafruit_NeoPixel(100, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
	sections[8] = Section(0, 30, strips[2]);
	sections[9] = Section(31, 49, strips[2]); //TODO: hack
        colorPulseEffect[2] = new ColorPulseEffect(sections, 8, 9);
        
	//RIGHT LEG
	strips[3] = new Adafruit_NeoPixel(50, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
	sections[10] = Section(0, 49, strips[3]);
        colorPulseEffect[3] = new ColorPulseEffect(sections, 10,10);

	//LEFT LEG
	strips[4] = new Adafruit_NeoPixel(50, LEFT_LEG_PIN, NEO_GRB + NEO_KHZ800);
	sections[11] = Section(0, 49, strips[4]);
        colorPulseEffect[4] = new ColorPulseEffect(sections, 11,11);

	//HEART
	strips[5] = new Adafruit_NeoPixel(240, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
	sections[12] = Section(0, 239, strips[5]);
        colorPulseEffect[5] = new ColorPulseEffect(sections, 12,12);

	// For now, i pass in the section that i work with
	//pulseEffect = new PulseEffect(sections);
        
        //colorPulseEffect->setSourceColor(Adafruit_NeoPixel::Color(10, 10, 10));
}

void tick() {
  tickActiveProgram();
  
}

void tickActiveProgram(void) {
	switch (activeEffect) {
		//BTN_REVERSE_PULSE_PIN
		case 0:
			//Serial.println("Reverse Pulse");
			break;
			//BTN_ONLY_HEART_PIN
		case 1:
			//Serial.println("Only Heart");
                        //pulseEffect->tick();
			break;
			//BTN_PULSE_PIN
		case 2:
			//Serial.println("Pulse");
                        for (int i=0;i<PARTS_NUM;i++) {
			  colorPulseEffect[i]->tick();
                        }
			break;
			//BTN_FIREWORKS_PIN
		case 3:
			Serial.println("Fireworks");
			break;
			//BTN_ELECTIC_SHOCK_PIN
		case 4:
			Serial.println("Electric Shock");
			break;
			//BTN_ELECTRIC_SPARKS_PIN
		case 5:
			Serial.println("Electric Sparks");
			break;
			//BTN_OVERRIDER_PIN
		case 6:
                        currentColor = colors[(currentColorIndex % 4)];
                        currentColorIndex++;
                        for(int i=0;i<PARTS_NUM;i++) {
                          colorPulseEffect[i]->setSourceColor(currentColor);
                        }
			Serial.println("Overrider");
			break;
	}
}

void loop() {
  updateEffectByButtons();
  //loopSonars();
  delay(800);
}


/* Hack - Buttons pins should be ordinal */
void updateEffectByButtons() {
	//Remember that there is also on/off button
	for (int i = 0; i < BUTTONS_NUM; i++) {
		int newState = digitalRead(BTN_A_PIN + i);
		if (buttons[i] == newState)
			continue;

		buttons[i] = newState;
		// the button state has changed!
		if (newState == LOW) {                // check if the button is pressed
			Serial.println("Button just pressed");
			noInterrupts();
			activeEffect = i;
			interrupts();
			break;
		} else {
			Serial.println("Button just released");
		}
	}

}

//void loopSonars() {
//	for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
//		if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
//              	        pingTimer[i]+= PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
//			if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
//                        sonars[currentSensor] = sonar[i].ping() / US_ROUNDTRIP_CM;
//			//sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
//			currentSensor = i;                          // Sensor being accessed.
////			sonars[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
////			sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
//		}
//	}
//}

//void echoCheck() { // If ping received, set the sensor distance to array.
//	if (sonar[currentSensor].check_timer())
//		sonars[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
//}

//void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
//	for (uint8_t i = 0; i < SONAR_NUM; i++) {
//		Serial.print(i);
//		Serial.print("=");
//		Serial.print(sonars[i]);
//		Serial.print("cm ");
//	}
//	Serial.println();
//}





