// ---------------------------------------------------------------------------
// Using multiple sensors with timer.
// Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 66ms (33 * 2 sensors = 66ms). The results
// are sent to the "oneSensorCycle" function which currentlyjust displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>

#define SONAR_NUM     1 // Number or sensors.
#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 400 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

#define LED_STRIP_1 8

#define COLORS_NUM 2

uint32_t colors[COLORS_NUM];
uint32_t currentColorIndex = 0;

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
uint32_t lastBrightness = 255;
uint8_t lastBrightnessStep = 40;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(10, 11, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  //NewPing(3, 2, MAX_DISTANCE)
};

Adafruit_NeoPixel strip(20, LED_STRIP_1, NEO_GRB + NEO_KHZ800);

long randRNumber;
long randGNumber;
long randBNumber;


void setup() {
  Serial.begin(115200);
  initColors();
  strip.begin();
  strip.show();
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void initColors() {
	colors[0] = Adafruit_NeoPixel::Color(200, 50, 0);
	colors[1] = Adafruit_NeoPixel::Color(170, 50, 0);
//	colors[2] = Adafruit_NeoPixel::Color(130, 60, 110);
//	colors[3] = Adafruit_NeoPixel::Color(21, 70, 92);
}
void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // The rest of your code would go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
//    if((cm[0] == cm[1]) && (cm[0]< 5) && (cm[0] > 0))
//    {
//        Serial.println("Rainbow time");
//        noInterrupts();
//        theaterChaseRainbow(10);
//    }

  uint32_t currentColor = getNextColor();
  portal(currentColor, 1);
    
    
//    Serial.print(i);
//    Serial.print("=");
//    Serial.print(cm[i]);
//    Serial.print("cm ");
  }
  //Serial.println();
}

uint32_t getNextColor() {
  uint32_t currentColor = colors[(currentColorIndex % COLORS_NUM)];
  currentColorIndex++;
  return currentColor;
}


void portal(uint32_t c, uint8_t wait) {  
  uint16_t numOfPixels = strip.numPixels();
  
  float normalize = float(cm[0] / (float)numOfPixels); //0..1
    //Serial.print("Normalize");
    //Serial.println(normalize);
    
  int pixelsToLight = numOfPixels - (normalize * (float)numOfPixels); //reverse it
  if(pixelsToLight <0)
    pixelsToLight = 0;

//      Serial.print("PixelsToLight2");
//  Serial.println(pixelsToLight2);
  //unsigned int pixelsToLight = cm[0] / 2;
  uint16_t startPixel = (numOfPixels/2) - (pixelsToLight / 2);

  Serial.print("Lighting");
  Serial.print(pixelsToLight);
  Serial.println(" Pixels. Starting with:");
   Serial.println(startPixel);
   
    for(uint16_t i=startPixel; i<=(startPixel+pixelsToLight); i++) {
      strip.setPixelColor(i, c);
    }
    
    lastBrightness -= lastBrightnessStep;
    strip.setBrightness(lastBrightness);
    //make outside pixels less bright
    for(uint16_t i=0;i<startPixel;i++) {
      strip.setPixelColor(i, strip.getPixelColor(i));
    }
    for(uint16_t i=(startPixel+pixelsToLight);i<numOfPixels;i++) {
      strip.setPixelColor(i, strip.getPixelColor(i));
    }

  strip.show();
}
