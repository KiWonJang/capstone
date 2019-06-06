#include <Wire.h>
#include <Adafruit_ADS1015.h>



Adafruit_ADS1115 ads(0x48);
float Voltage = 0.0;
const static float Vin = 5;
const static float Rref = 986;
float Rx;
//float Vout;
double y;
double pre_y;
float tau = 0.5;
float ts = 0.01;
int16_t adc0;

void setup(void) 
{
  Serial.begin(9600);  
  ads.begin();
  adc0 = ads.readADC_SingleEnded(1);
  pre_y = (adc0 * 0.1875)/1000;
}

void loop(void) 
{
  adc0 = ads.readADC_SingleEnded(1);
  Voltage = (adc0 * 0.1875)/1000;
  y = ( tau * pre_y + ts * Voltage ) /(tau + ts) ;
  pre_y = y;
  Rx = y*Rref/(Vin-y);
//  Serial.print("AIN0: "); 
//  Serial.print(adc0);
//  Serial.print("\tVoltage: ");
  Serial.println(Rx, 4);  
//  Serial.println();
  
  delay(ts*1000);
}
