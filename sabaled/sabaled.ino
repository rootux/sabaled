#include <Adafruit_NeoPixel.h>
#include <PulseEffect.h>
#include <Sabalib.h>
#include <TimerThree.h>
#include <NewPing.h>

#define RIGHT_TORSO_HAND_STICK_PIN 3
#define CHEST_HEAD_BEARD_PIN 4
#define LEFT_TORSO_HAND_PIN 5
#define LEFT_LEG_PIN 6
#define RIGHT_LEG_PIN 7
#define HEART_PIN 8

#define SONAR_NUM 2 //number of sonars
#define MAX_DISTANCE 250 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 100 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int sonars[SONAR_NUM];     // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(10, 11, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(12, 13, MAX_DISTANCE)
};

Section sections[13];
Adafruit_NeoPixel* strips[6];
PulseEffect* pulseEffect;

void setup() {
  Serial.begin(9600);  
  initSonars();
  initSections();
  Timer3.initialize(10000); // blinkLED to run every 0.01 second
  Timer3.attachInterrupt(tick); 
}

void initSonars(void) {
  // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  pingTimer[0] = millis() + 75;
  for (uint8_t i = 1; i < SONAR_NUM; i++) {// Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  }
}

void initSections(void) {
  //RIGHT TORSO HAND STICK
  strips[0] = new Adafruit_NeoPixel(200, RIGHT_TORSO_HAND_STICK_PIN, NEO_GRB + NEO_KHZ800);
  sections[0] = Section(0, 49, strips[0]);
  sections[1] = Section(50, 99, strips[0]);
  sections[2] = Section(100, 149, strips[0]);
  sections[3] = Section(150, 199, strips[0]);
  
  //CHEST HEAD BEARD
  strips[1] = new Adafruit_NeoPixel(960, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
  sections[4] = Section(0, 239, strips[1]);
  sections[5] = Section(240, 479, strips[1]);
  sections[6] = Section(480, 719, strips[1]);
  sections[7] = Section(720, 959, strips[1]);
  
  //LEFT TORSO HAND
  strips[2] = new Adafruit_NeoPixel(100, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
  sections[8] = Section(0, 49, strips[2]);
  sections[9] = Section(50, 99, strips[2]);

  //RIGHT LEG
  strips[3] = new Adafruit_NeoPixel(50, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
  sections[10] = Section(0, 49, strips[3]);
  
  //LEFT LEG
  strips[4] = new Adafruit_NeoPixel(50, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
  sections[11] = Section(0, 49, strips[4]);
  
  //HEART
  strips[5] = new Adafruit_NeoPixel(240, CHEST_HEAD_BEARD_PIN, NEO_GRB + NEO_KHZ800);
  sections[12] = Section(0, 239, strips[5]);
  
  pulseEffect = new PulseEffect(sections);
  
  
}

void tick() {
  tickActiveProgram();
}

void tickActiveProgram(void) {
    // reverse pulse effect
    // only heart pulse effect
    // pulse effect pink to purple
    pulseEffect->tick();
}

void loop() {
  loopSonars();
}

void loopSonars() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      sonars[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    sonars[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonars[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
