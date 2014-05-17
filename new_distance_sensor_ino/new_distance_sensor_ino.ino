#include <Maxbotix.h>
#include <RxSoftwareSerial.h>

/*
  Maxbotix simple test

  Instructions:
  - At least one of: (comment the appropriate code below)
    * PW is digital pin 8
    * TX is digital pin 6
    * AN is analog pin A0
  - Change code below according to your model (LV, XL and
  HRLV supported)

*/

Maxbotix rangeSensorPW(8, Maxbotix::PW, Maxbotix::LV);
//Maxbotix rangeSensorTX(6, Maxbotix::TX, Maxbotix::LV);
//Maxbotix rangeSensorAD(A0, Maxbotix::AN, Maxbotix::LV);

int avgrange=10;//Quantity of values to average (sample size)

#define HIGHEST_RANGE_NORMALIZER 30 //the error from highest range

int sum,range,cm,currentAvg;

int highestValue = 0; //Stores the highest read value - normalize it to remove errors

void setup()
{
  Serial.begin(9600);
}

void loop()
{ 
  sum = 0;
  currentAvg = avgrange;
  for(int i = 0; i < avgrange ; i++)
  {
    //Used to read in the analog voltage output that is being sent by the MaxSonar device.
    range = rangeSensorPW.getRange();
    
    //update highest value
    if(range > highestValue) {
      highestValue = range;
      Serial.print("new highestValue:");
      Serial.println(range);
    }
    
    //ignore `if in highest value range
    if(isInHighestRange(range)) {
      currentAvg--;
      //don't add to sum
    }else {
      sum += range;
      delay(10);
    }
  } 
  cm = sum / currentAvg;
  if(cm > 0) {
    Serial.print(cm);
    Serial.print(" cm");
  }
  
  // TX
//  start = millis();
//  Serial.print("TX: ");
//  Serial.print(rangeSensorTX.getRange());
//  Serial.print("cm - ");
//  Serial.print(millis() - start);
//  Serial.println("ms");
  
// AD
//  start = millis();
//  Serial.print("AD: ");
//  Serial.print(rangeSensorAD.getRange());
//  Serial.print("cm - ");
//  Serial.print(millis() - start);
//  Serial.println("ms");
  
  Serial.println();
  delay(50);
}

//check if given range is in the area of highestValue
boolean isInHighestRange(int range) {
  boolean result = (range + HIGHEST_RANGE_NORMALIZER > highestValue);
  //debugging
  if(result) {
    //Serial.print("Just cleaned:");
    //Serial.println(range);
  }
  return result;
}
