#include <Adafruit_NeoPixel.h>
#include <PulseEffect.h>
#include <Sabalib.h>
#include <TimerThree.h>

#define RIGHT_TORSO_HAND_STICK_PIN 3
#define CHEST_HEAD_BEARD_PIN 4
#define LEFT_TORSO_HAND_PIN 5
#define LEFT_LEG_PIN 6
#define RIGHT_LEG_PIN 7
#define HEART_PIN 8

Section sections[13];
Adafruit_NeoPixel* strips[6];
PulseEffect* pulseEffect;

void setup() {
  Serial.begin(9600);  
  initSections();
  Timer3.initialize(10000); // blinkLED to run every 0.01 second
  Timer3.attachInterrupt(tick); 
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
  
}
