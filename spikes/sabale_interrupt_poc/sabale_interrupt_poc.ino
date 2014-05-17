#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#include <Sabalib.h>

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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_GRB + NEO_KHZ400);

PartBase part = PartBase();

const int led = LED_BUILTIN;  // the pin with a LED

void setup(void)
{
  strip.begin();
  strip.show();
  pinMode(led, OUTPUT);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(blinkLED); // blinkLED to run every 0.01 second
  Serial.begin(9600);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

int blink_direction = 1;
int pulse_step = 10;
int pulse_max = 60;
int pulse_index = 0;
int pulse_current_level = 0;
void blinkLED(void)
{

  
  pulse_current_level += pulse_step;
  // go up
  int temp_level = pulse_current_level;
  int temp_index = pulse_index;
  while(temp_level > 0)
  {
     strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
     temp_index++;
     temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step);
  }  

  // trying to come down, didn't work
  /*

  // come down
  temp_index = pulse_index - 1;
  temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step);
  while(temp_level > 0)
  {
     strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
     temp_index--;
     temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step);
  }
  
  */
 
  strip.show();
  
  if (pulse_current_level == pulse_max) 
  {
     pulse_index++;
  }
   
   
   
   
   
   
   
   
   
   
   
   
//     if (ledState == LOW) {
//    ledState = HIGH;
//  } else {
//    ledState = LOW;
//  }
//  digitalWrite(led, ledState);
   
   
   
   
//  if (blinkCount >= 70) {
//    blink_direction = -1;
//  }
//  if(blinkCount <= 0) {
//     blink_direction = 1; 
//  }
//    
//  blinkCount = blinkCount + (1 * blink_direction);  
//
//  strip.setPixelColor(1, strip.Color(0, 0, blinkCount));
//  strip.show();
}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
  unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  blinkCopy = blinkCount;
  interrupts();

  Serial.print("blinkCount = ");
  Serial.println(blinkCopy);
  delay(100);
}

