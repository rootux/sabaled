#include <Adafruit_NeoPixel.h>
#include <PulseEffect.h>
#include <ColorPulseEffect.h>
#include <ColorWipeEffect.h>
#include <HeartBeatEffect.h>
#include <Sabalib.h>
#include <TimerThree.h>
#include <NewPing.h>

#define RIGHT_TORSO_HAND_STICK_PIN 3 //A
#define CHEST_HEAD_BEARD_PIN 4 //B
#define LEFT_TORSO_HAND_PIN 5 //C
#define LEFT_LEG_PIN 6 //D
#define RIGHT_LEG_PIN 7 //E
#define HEART_PIN 8 //F

#define BUTTONS_NUM 9
#define PARTS_NUM 6
#define COLORS_NUM 4

#define TIMER_INTERVAL 100000
#define DEBUGI true
 
#define OVERRIDER_PIN 7
//Remember that there is also on/off button
#define BTN_A_PIN 21
#define BTN_B_PIN 22
#define BTN_C_PIN 23
#define BTN_D_PIN 24
#define BTN_E_PIN 25
#define BTN_F_PIN 26
#define BTN_G_PIN 27
#define PUSH_BTN_A_PIN 28
#define PUSH_BTN_B_PIN 29
#define SLIDER_A_PIN 30
#define SLIDER_B_PIN 31
#define SLIDER_C_PIN 32

uint32_t sliderA = 0;
uint32_t sliderB = 0;
uint32_t sliderC = 0;

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
ColorPulseEffect *colorPulseEffect[PARTS_NUM];
ColorWipeEffect *colorWipeEffect[PARTS_NUM];
PulseEffect *pulseEffect[PARTS_NUM];
HeartBeatEffect *heartBeatEffect[PARTS_NUM];

void setup() {
        if(DEBUGI) {
	  Serial.begin(9600);
        }
        SerialPrint("Booting up...");
        initColors();
        SerialPrintln("Init colors success...");        
	//initSonars();
	initSections();
        SerialPrintln("Init sections success...");
        testSabaleAndStartTimer();
}

void SerialPrint(char* str) {
  if(DEBUGI) {
    Serial.print(str);
  }
}

void SerialPrintln(char* str) {
  if(DEBUGI) {
    Serial.println(str);
  }
}

void SerialPrintln(int str) {
  if(DEBUGI) {
    Serial.println(str);
  }
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

void initSections(void) {
        uint8_t current_part = 0;
        
	//RIGHT TORSO HAND STICK
	strips[current_part] = new Adafruit_NeoPixel(200, RIGHT_TORSO_HAND_STICK_PIN, NEO_GRB + NEO_KHZ800);
	sections[0]= Section(0, 49, strips[current_part]);
	sections[1] = Section(50, 99, strips[current_part]);
	sections[2] = Section(100, 149, strips[current_part]);
	sections[3] = Section(150, 199, strips[current_part]);

        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 0, 3);
        pulseEffect[current_part] = new PulseEffect(sections,0,3);
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,0,3);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,0,3);
        
        SerialPrintln("Right Torso Hand Stick inited...");
        delay(50);
        current_part++;
        
	//CHEST HEAD BEARD
	strips[current_part] = new Adafruit_NeoPixel(960, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
	sections[4] = Section(0, 239, strips[current_part]);
	sections[5] = Section(240, 479, strips[current_part]);
	sections[6] = Section(480, 719, strips[current_part]);
	sections[7] = Section(720, 959, strips[current_part]);
        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 4, 7);
        pulseEffect[current_part] = new PulseEffect(sections,4,7);
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,4,7);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,4,7);
        
        SerialPrintln("Chest Head Beard inited...");
        delay(50);
        current_part++;

	//LEFT TORSO HAND
	strips[current_part] = new Adafruit_NeoPixel(100, LEFT_TORSO_HAND_PIN, NEO_GRB + NEO_KHZ800);
	sections[8] = Section(0, 30, strips[current_part]);
	sections[9] = Section(31, 49, strips[current_part]); //TODO: hack
        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 8, 9);
        pulseEffect[current_part] = new PulseEffect(sections,8,9);
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,8,9);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,8,9);
        
        SerialPrintln("Left torso hand inited...");
        delay(50);
        current_part++;
        
	//RIGHT LEG
	strips[current_part] = new Adafruit_NeoPixel(50, RIGHT_LEG_PIN, NEO_GRB + NEO_KHZ800);
	sections[10] = Section(0, 49, strips[current_part]);
        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 10,10);
        pulseEffect[current_part] = new PulseEffect(sections,10,10);
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,10,10);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,10,10);
        
        SerialPrintln("Right leg inited...");
        delay(50);
        current_part++;
        
	//LEFT LEG
	strips[current_part] = new Adafruit_NeoPixel(50, LEFT_LEG_PIN, NEO_GRB + NEO_KHZ800);
	sections[11] = Section(0, 49, strips[current_part]);
        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 11,11);
        pulseEffect[current_part] = new PulseEffect(sections,11,11);    
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,11,11);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,11,11);        

        SerialPrintln("Left leg inited...");
        delay(50);
        current_part++;

	//HEART
	strips[current_part] = new Adafruit_NeoPixel(240, HEART_PIN, NEO_GRB + NEO_KHZ800);
	sections[12] = Section(0, 239, strips[5]);
        colorPulseEffect[current_part] = new ColorPulseEffect(sections, 12,12);
        pulseEffect[current_part] = new PulseEffect(sections,12,12);
        heartBeatEffect[current_part] = new HeartBeatEffect(sections,12,12);
        colorWipeEffect[current_part] = new ColorWipeEffect(sections,12,12);
        //colorPulseEffect->setSourceColor(Adafruit_NeoPixel::Color(10, 10, 10));
        
        SerialPrintln("Heart is pumping...");
        delay(50);
        current_part++;

}

void tick() {
  tickActiveProgram();
}

/**
* Perform an init
*/
void testSabaleAndStartTimer() {
  SerialPrintln("Starting test");
  delay(50);
  for(int i=0; i< PARTS_NUM;i++) {
      SerialPrint("Testing part:");
      SerialPrintln(i);
      
     strips[i]->begin();
     strips[i]->show();
     for(int j=0;j<strips[i]->numPixels();j++) {
       strips[i]->setPixelColor(j, colors[0]);
     }
     strips[i]->show();
     delay(500);
     //clear
     for(int j=0;j<strips[i]->numPixels();j++) {
       strips[i]->setPixelColor(j, 0);
     }
     strips[i]->show();
  }
  SerialPrint("Test success! go saba go!");
  Timer3.initialize(TIMER_INTERVAL); // blinkLED to run every 0.01 second
  Timer3.attachInterrupt(tick); //Start the timer
}

void tickActiveProgram(void) {
	switch (activeEffect) {
		case 0: //REVERSE PULSE
			SerialPrintln("Reverse Pulse");
			break;
		case 1: //Only Heart
			SerialPrintln("Only Heart");
                        heartBeatEffect[5]->tick();
                        //pulseEffect->tick();
                        //for (int i=0;i<PARTS_NUM;i++) {
                          //colorPulseEffect[i]->tick();
                        //}
			break;
			
		case 2: //PULSE
                        SerialPrintln("Pulse");
                        //colorPulseEffect[5]->tick();
		        //SerialPrintln("Pulse");
                        //for (int i=0;i<PARTS_NUM;i++) {
                          //pulseEffect[i]->tick();
			  colorPulseEffect[5]->tick();
                            //heartBeatEffect[i]->tick();
                            
                        //}
			break;
		case 3:  //Fireworks
			SerialPrintln("Fireworks");
                        colorWipeEffect[5]->tick();
			break;
		case 4: //BTN_ELECTIC_SHOCK_PIN
			SerialPrintln("Electric Shock");
			break;
		case 5: //BTN_ELECTRIC_SPARKS_PIN
			SerialPrintln("Electric Sparks");
			break;
			
		case 6: //BTN_OVERRIDER_PIN
                        SerialPrintln("Overrider");
                        currentColor = colors[(currentColorIndex % 4)];
                        currentColorIndex++;
                        for(int i=0;i<PARTS_NUM;i++) {
                          colorPulseEffect[i]->setSourceColor(currentColor);
                        }
			break;
	}
}

void loop() {
  updateEffectByButtons();
  //loopSonars();
  delay(1000);
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
			SerialPrintln("Button just pressed");
			noInterrupts();
			activeEffect = i;
			interrupts();
			break;
		} else {
			SerialPrintln("Button just released");
		}
	}
        
        //CHECK IF OVERRIDER IS UP
        if(buttons[OVERRIDER_PIN] == LOW)
        {
          //Read the 3 sliders        
          noInterrupts();
          sliderA = analogRead(SLIDER_A_PIN);
          sliderB = analogRead(SLIDER_B_PIN);
          sliderC = analogRead(SLIDER_C_PIN);
          interrupts();
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





