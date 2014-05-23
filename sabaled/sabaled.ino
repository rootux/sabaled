/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#include <Adafruit_NeoPixel.h>

#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10

#define NUM_OF_STRIPS 3

#define LED_STRIP_1 6
#define LED_STRIP_2 5
#define LED_STRIP_3 4

#define DEBUG_FULL_POWER false

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(19, LED_STRIP_1, NEO_GRB + NEO_KHZ400);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(19, LED_STRIP_2, NEO_GRB + NEO_KHZ400);
//Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(19, LED_STRIP_3, NEO_GRB + NEO_KHZ400);

Adafruit_NeoPixel* strips[NUM_OF_STRIPS];
    
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

long randRNumber;
long randGNumber;
long randBNumber;
    
void setup() {

strips[0] = new Adafruit_NeoPixel(19, LED_STRIP_1, NEO_GRB + NEO_KHZ400);
strips[1] = new Adafruit_NeoPixel(19, LED_STRIP_2, NEO_GRB + NEO_KHZ400);
strips[2] = new Adafruit_NeoPixel(19, LED_STRIP_3, NEO_GRB + NEO_KHZ400);

  //Distance
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  
  //Leds
  for (int i=0; i < NUM_OF_STRIPS; i++) {
    strips[i]->begin();
    strips[i]->show(); // Initialize all pixels to 'off'
  }
}

void loop() {
 play_distance();
}

void play_distance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; //29.1 to convert to centimeters
 
  
  if (distance > 0) {
    long stepi;
    
    stepi = distance / 10;
    
    randRNumber = random(255);
    randGNumber = random(255);
    randBNumber = random(255);
    
    for (int i=0; i < NUM_OF_STRIPS; i++) {
      //theaterChaseRainbow(*strips[i],20);
      
      if(DEBUG_FULL_POWER) {
        colorWipe(*strips[i], strips[i]->Color(255, 255, 255), 25, 25);
      }else {
        colorWipe(*strips[i], strips[i]->Color(randRNumber, randGNumber, randBNumber), 20, stepi);
      }
    }
  }
  
  
  if (distance < 200) {  // This is where the LED On/Off happens
      digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
      digitalWrite(led2,LOW);
  }
  else {
      digitalWrite(led,LOW);
      digitalWrite(led2,HIGH);
  }
  if (distance >= 400 || distance <= 0){
      Serial.println("Out of range:" + distance);
  }
  else {
      Serial.print(distance);
      Serial.println(" cm");
  }
  delay(200);
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel &stripi, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < stripi.numPixels(); i=i+3) {
          stripi.setPixelColor(i+q, Wheel(stripi, (i+j) % 255));    //turn every third pixel on
        }
        stripi.show();
       
        delay(wait);
       
        for (int i=0; i < stripi.numPixels(); i=i+3) {
          stripi.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel &stripi, uint32_t c, uint8_t wait,uint8_t num_pixels_to_lit) {
  for(uint16_t i=0; i<stripi.numPixels(); i++) {
      if(i <= num_pixels_to_lit) {
        stripi.setPixelColor(i, c);
      }else //turn off
      {
          stripi.setPixelColor(i, stripi.Color(0, 0, 0));
      }
      stripi.show();
      delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel &stripi, byte WheelPos) {
  if(WheelPos < 85) {
   return stripi.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return stripi.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return stripi.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

