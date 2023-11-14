#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <TimeLib.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <time.h>

#define DHTPIN 6
#define ONEWIREPIN 2
#define LM35DZ A0
#define THERMISTOR A4
 #define REF_OHM 100000
//#define REF_OHM 1990
#define B 3950

// source of table: https://www.tme.eu/Document/f9d2f5e38227fc1c7d979e546ff51768/NTCM-100K-B3950.pdf
float arr[] = { 11093.8,10156.1,9311.69,8549.81,7861.2,7237.73,6672.31,6158.7,
                5691.44,5265.69,4877.2,4522.23,4197.42,3899.83,3626.83,3376.08,
                3145.49,2933.2,2737.53,2556.98,2390.22,2236.03,2093.33,1961.13,
                1838.54,1724.78,1619.1,1520.85,1429.44,1344.32,1265,1191.02,
                1121.98,1057.5,997.241,940.885,888.148,838.764,792.494,749.115,
                708.422,670.228,634.359,600.657,568.973,539.171,511.127,484.723,
                459.851,436.413,414.316,393.473,373.806,355.239,337.705,321.14,
                305.482,290.679,276.676,263.427,250.886,239.012,227.764,217.106,
                207.005,198.53,188.343,179.724,171.545,163.78,156.407,149.403,
                142.748,136.423,130.41,124.692,119.253,114.078,109.152,104.464,
                100,95.747,91.697,87.837,84.157,80.65,77.305,74.115,71.072,68.167,
                65.395,62.749,60.222,57.809,55.503,53.3,51.195,49.183,47.259,
                45.419,43.659,41.975,40.364,38.822,37.346,35.84,34.58,33.284,
                32.043,30.853,29.713,28.62,27.573,26.568,25.605,24.681,23.794,
                22.944,22.127,21.344,20.592,19.869,19.175,18.509,17.869,17.253,
                16.662,16.094,15.548,15.022,14.517,14.031,13.564,13.114,12.681,
                12.265,11.864,11.478,11.106,10.748,10.404,10.071,9.751,9.443,
                9.146,8.859,8.583,8.316,8.06,7.812,7.573,7.342,7.12,6.905,6.698,
                6.498,6.304,6.118,5.937,5.763,5.595,5.433,5.276,5.124,4.977,4.836,
                4.698,4.566,4.438,4.314,4.194,4.077,3.965,3.856,3.751,3.649,3.551,
                3.455,3.363,3.273,3.186};

OneWire oneWire(ONEWIREPIN);
DS18B20 sensor(&oneWire);

DHT_Unified dht(DHTPIN, DHT11);

void print_measurement(int32_t time, float dht, float DS18B20, float lm35dz, float termistor, float humidity){
  Serial.print("-->");
  Serial.print(time);
  Serial.print(",  ");
  Serial.print(dht);
  Serial.print(",  ");
  Serial.print(DS18B20);
  Serial.print(",  ");
  Serial.print(lm35dz);
  Serial.print(",  ");
  Serial.print(termistor);
  Serial.print(",  ");
  Serial.print(humidity);
  Serial.println();
}

void print_measurement(float dht, float DS18B20, float lm35dz, float termistor, float humidity){
  Serial.print(",  ");
  Serial.print(dht);
  Serial.print(",  ");
  Serial.print(DS18B20);
  Serial.print(",  ");
  Serial.print(lm35dz);
  Serial.print(",  ");
  Serial.print(termistor);
  Serial.print(",  ");
  Serial.print(humidity);
  Serial.println();
}

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  dht.begin();

  if (sensor.begin() == false)
  {
    Serial.println("Error: Could not find sensor.");
    sensor.setResolution(12);
    sensor.requestTemperatures();
  }

  delayMS = 5000-2000; // for short
  delayMS = 30000; // for medium
  delayMS = 5*60000; // for long
}

#define T0 25+273.15
float VR,ln;

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float dhtTemp = event.temperature;
  dht.humidity().getEvent(&event);

  int lm35dz_val = analogRead(LM35DZ);
  float temp_lm35dz = 500*lm35dz_val/1023.0;

  digitalWrite(4,HIGH);
  int thermistor_val = 0;
  delay(1000);
  for (int i = 0; i < 5; i++)
  {
    thermistor_val += analogRead(THERMISTOR);
  }
  delay(1000);
  digitalWrite(4,LOW);
  float temp_therm = thermistor_val/1023.0; // thermistor_val should be divided by 5 for one measurement, 
                                            //   but multiplied by 5 for Vcc ==>> no multiplication needed
  temp_therm = REF_OHM*temp_therm/(5-temp_therm);
  temp_therm /=1000.0;
  for(int i = 1;i<181;i++){
    if(arr[i]<=temp_therm){
      temp_therm=i-55-(arr[i]-temp_therm)/(arr[i]-arr[i-1]);
      break;
    }
  }


  if (sensor.isConversionComplete())
  {
    Serial.print("inside:\t");
    Serial.println(sensor.getTempC(),1);
    sensor.requestTemperatures();
  }
  print_measurement(dhtTemp,sensor.getTempC(),temp_lm35dz,temp_therm,event.relative_humidity);
  delay(delayMS);
  
}