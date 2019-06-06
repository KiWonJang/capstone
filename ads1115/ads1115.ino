#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define ARRAY_SIZE 100
#define SENSOR_NUM 7

#define CALIBRATION_1 1
#define CALIBRATION_2 2
#define MAIN 3

// for logic control
static int phase;
// for calibration
static double caliArr[SENSOR_NUM][ARRAY_SIZE];
static double initVal[SENSOR_NUM];
static double deltaVal[SENSOR_NUM];
static int index;
// for low-pass filter
const static double tau = 0.5;
const static double ts = 0.01;
static double y[SENSOR_NUM];
static double pre_y[SENSOR_NUM];
// for sensor input
Adafruit_ADS1115 ads0(0x48);
Adafruit_ADS1115 ads1(0x49);
const static double Vin = 5;
const static double Rref = 986;
static double Voltage = 0.0;
static int16_t adc0;

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

double getResistance(int sensor){
  if(sensor < 4){
    adc0 = ads.readADC_SingleEnded(sensor);
    Voltage = (adc0 * 0.1875)/1000;
    y[sensor] = ( tau * pre_y[sensor] + ts * Voltage ) /(tau + ts) ;
    pre_y[sensor] = y[sensor];
    
  }else{
    adc0 = ads.readADC_SingleEnded(sensor - 3);
    Voltage = (adc0 * 0.1875)/1000;
    y[sensor] = ( tau * pre_y[sensor] + ts * Voltage ) /(tau + ts) ;
    pre_y[sensor] = y[sensor];
  }
  
  return y[sensor]*Rref/(Vin-y[sensor]);
}

void setup(void) 
{
  Serial.begin(9600);  
  ads0.begin();
  ads1.begin();
  phase = CALIBRATION_1;
  index = 0;
}

void loop(void) 
{
  if(0){ // timeout
    if(phase == CALIBRATION_1){
      phase = CALIBRATION_2;
      for(int i=0; i<SENSOR_NUM; i++)
        initVal[i] = getAverage(caliArr[i]);
    }else if (phase == CALIBRATION_2){
      phase = MAIN;
      for(int i=0; i<SENSOR_NUM; i++)
        deltaVal[i] = getAverage(caliArr[i]);
    }
  }
  
  switch(phase){
    case CALIBRATION_1:
      for(int i=0; i<SENSOR_NUM; i++){
        calibration(caliArr[i], getResistance(i));
      }
      break;
    case CALIBRATION_2:
      for(int i=0; i<SENSOR_NUM; i++){
        calibration(caliArr[i], getResistance(i));
      }
      break;
    case MAIN:
      break;
  }
  
//  Serial.print("AIN0: "); 
//  Serial.print(adc0);
//  Serial.print("\tVoltage: ");
//  Serial.println(Rx, 4);  
//  Serial.println();
  
  delay(ts*1000);
}
