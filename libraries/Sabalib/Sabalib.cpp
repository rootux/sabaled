#include "Sabalib.h"

Section::Section(int startIndex, int endIndex, Adafruit_NeoPixel* strip) {
	this->strip = strip;
}

void Section::begin(void) {
	Serial.println("Init section");
	strip->begin();
}

void Section::show(void) {
	strip->show();
}

Section::~Section() {

}

PartBase::PartBase(Section *sections, int numberOfSections) : 
	sections(sections), numberOfSections(numberOfSections)
{

}

PartBase::~PartBase()
{

}
			
void PartBase::tick()
{
	
}

BogusPart::BogusPart(Section *sections, int numberOfSections) : 
	PartBase(sections, numberOfSections)
{
	this->blink_direction = 1;
	this->pulse_step = 10;
  	this->pulse_max = 60;
  	this->pulse_index = 0;
  	this->pulse_current_level = 0;
}


BogusPart::~BogusPart()
{

}

void BogusPart::tick()
{
 	Adafruit_NeoPixel* strip = sections->strip;
 	// uint8_t* pixels = strip.getPixels();
 	// Serial.println('Pixels array:');
 	// for(int i=0;i<strip.numPixels();i++) {
 	// 	Serial.print(pixels[i]);
 	// }
  if (pulse_current_level < pulse_max) 
  {
      pulse_current_level += pulse_step;  
  }
  
  // go up
  int temp_level = pulse_current_level;
  int temp_index = pulse_index;
  while(temp_level > 0)
  {
  	Serial.println(temp_index);
     strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
     temp_index++;
     temp_level = pulse_current_level - ((temp_index - pulse_index) * pulse_step);
  }  
  
  if(pulse_current_level == pulse_max) 
  {
      temp_index = pulse_index - 1;
      temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step);
      while(temp_index >= 0 && temp_level >= 0)
      {
         strip->setPixelColor(temp_index, strip->Color(temp_level, 0, 0));
         temp_index--;
         temp_level = pulse_current_level - ((pulse_index - temp_index) * pulse_step);
      }
  }
  
  strip->show();
 
  if (pulse_current_level == pulse_max) 
  {
      pulse_index++;
  }
   
  if (pulse_index == strip->numPixels())
  {
      pulse_current_level = 0;
      pulse_index = 0;
  }
}
