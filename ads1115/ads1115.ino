#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define ARRAY_SIZE 100
#define SENSOR_NUM 7

Adafruit_ADS1115 ads(0x48);
int phase;
float Voltage = 0.0;
const static float Vin = 5;
const static float Rref = 986;
float Rx;
static double y[7];
static double pre_y[7];
float tau = 0.5;
float ts = 0.01;
int16_t adc0;
static double caliArr[7][ARRAY_SIZE];
static double initVal[7];
static double deltaVal[7];

void calibration(double arr[], double value){
  index = (index + 1) % ARRAY_SIZE;
  arr[index] = value;
}

double getAverage(double arr[]){
  double sum = 0;
  for(int i=0; i<ARRAY_SIZE; i++)
    sum += arr[i];
  return sum / ARRAY_SIZE;
}

double getSensorData(int sensor){
  
}

void setup(void) 
{
  Serial.begin(9600);  
  ads.begin();
  adc0 = ads.readADC_SingleEnded(1);
  pre_y = (adc0 * 0.1875)/1000;
  phase = CALIBRATION_1;
}

void loop(void) 
{
  if(0){ // timeout
    if(phase == CALIBRATION_1){
      phase = CALIBRATION_2;
      for(int i=0; i<SENSOR_SIZE; i++)
        initVal[i] = getAverage(caliArr[i]);
    }else if (phase == CALIBRATION_2){
      phase = MAIN;
      for(int i=0; i<SENSOR_SIZE; i++)
        deltaVal[i] = getAverage(caliArr[i]);
    }
  }
  switch(phase){
    case CALIBRATION_1:
      for(int i=0; i<SENSOR_NUM; i++){
        calibration(caliArr[i], getSensorData(i));
      }
      break;
    case CALIBRATION_2:
      for(int i=0; i<SENSOR_NUM; i++){
        calibration(caliArr[i], getSensorData(i));
      }
      break;
    case MAIN:
      break;
  }
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

void getResistance(){
  
}
