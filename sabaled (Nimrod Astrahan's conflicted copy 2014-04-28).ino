#include <Adafruit_NeoPixel.h>

//#define trigPin 13
//#define echoPin 12
//#define led 11
//#define led2 10
#define PIN 5


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(21, PIN, NEO_GRB + NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
//  //Distance
//  Serial.begin (9600);
//  pinMode(trigPin, OUTPUT);
//  pinMode(echoPin, INPUT);
//  pinMode(led, OUTPUT);
//  pinMode(led2, OUTPUT);
//
  //Leds
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
// play_distance();
    colorWipe(strip.Color(255, 0, 0), 50,21); // Red
}

//void play_distance() {
//  long duration, distance;
//  digitalWrite(trigPin, LOW);  // Added this line
//  delayMicroseconds(2); // Added this line
//  digitalWrite(trigPin, HIGH);
////  delayMicroseconds(1000); - Removed this line
//  delayMicroseconds(10); // Added this line
//  digitalWrite(trigPin, LOW);
//  duration = pulseIn(echoPin, HIGH);
//  distance = (duration/2) / 29.1; //29.1 to convert to centimeters
//
//
//  if (distance > 0) {
//    long stepi;
//
//    stepi = distance / 10;
//
//    colorWipe(strip.Color(255, 0, 0), 50,5); // Red
//  }
//
//
//  if (distance < 200) {  // This is where the LED On/Off happens
//      digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
//      digitalWrite(led2,LOW);
//  }
//  else {
//      digitalWrite(led,LOW);
//      digitalWrite(led2,HIGH);
//  }
//  if (distance >= 400 || distance <= 0){
//      Serial.println("Out of range:" + distance);
//  }
//  else {
//      Serial.print(distance);
//      Serial.println(" cm");
//  }
//  delay(200);
//}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait,uint8_t num_pixels_to_lit) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      if(i <= num_pixels_to_lit) {
        strip.setPixelColor(i, c);
      }else //turn off
      {
          strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
