#include <TinyWireS84.h>
#include "RunningMedian.h"

#define led_pin0 7
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define detector_pin0 0
#define detector_pin1 1
#define detector_pin2 2
#define detector_pin3 3

#define i2c_address 0xF

struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
};

void setup()
{
  pinMode(led_pin0, OUTPUT);
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
  
  TinyWireS.begin(i2c_address);
  TinyWireS.onRequest(on_request);
}

RunningMedian ir_readings[4];
RunningMedian dist_readings[4];

void readSensors(void)
{
  
  int preVal = analogRead(detector_pin0);
  ir_readings[0].add(preVal);
  digitalWrite(led_pin0, HIGH);
  delay(1);
  int newVal = analogRead(detector_pin0);
  if(preVal < newVal) 
    dist_readings[0].add((newVal-preVal));
  digitalWrite(led_pin0, LOW);

  
  preVal = analogRead(detector_pin1);
  ir_readings[1].add(preVal);
  digitalWrite(led_pin1, HIGH);
  delay(1);
  newVal = analogRead(detector_pin1);
  if(preVal < newVal) 
    dist_readings[1].add((newVal-preVal));
  digitalWrite(led_pin1, LOW);
  
  preVal = analogRead(detector_pin2);
  ir_readings[2].add(preVal);
  digitalWrite(led_pin2, HIGH);
  delay(1);
  newVal = analogRead(detector_pin2);
  if(preVal < newVal) 
    dist_readings[2].add((newVal-preVal));
  digitalWrite(led_pin2, LOW);
  
  preVal = analogRead(detector_pin3);
  ir_readings[3].add(preVal);
  digitalWrite(led_pin3, HIGH);
  delay(1);
  newVal = analogRead(detector_pin3);
  if(preVal < newVal) 
    dist_readings[3].add((newVal-preVal));
  digitalWrite(led_pin3, LOW);
}

void sendData(void)
{
  sensor_data_t ir_temp;
  ir_temp.sensor0 = ir_readings[0].getMedian();
  ir_temp.sensor1 = ir_readings[1].getMedian();
  ir_temp.sensor2 = ir_readings[2].getMedian();
  ir_temp.sensor3 = ir_readings[3].getMedian();

  sensor_data_t distance_temp;
  distance_temp.sensor0 = dist_readings[0].getMedian();
  distance_temp.sensor1 = dist_readings[1].getMedian();
  distance_temp.sensor2 = dist_readings[2].getMedian();
  distance_temp.sensor3 = dist_readings[3].getMedian();

  uint8_t* dist = (uint8_t*)&distance_temp;
  uint8_t* ir = (uint8_t*)&ir_temp;

  TinyWireS.send(dist[0]);
  TinyWireS.send(dist[1]);
  TinyWireS.send(dist[2]);
  TinyWireS.send(dist[3]);
  TinyWireS.send(dist[4]);

  TinyWireS.send(ir[0]);
  TinyWireS.send(ir[1]);
  TinyWireS.send(ir[2]);
  TinyWireS.send(ir[3]);
  TinyWireS.send(ir[4]);
}

void pulse(int delays)
{
  digitalWrite(5, HIGH);
  delay(delays);
  digitalWrite(5, LOW);
  delay(delays);
}

void loop()
{
  ;
}

void on_request(void)
{
  readSensors();
  digitalWrite(5, HIGH);
  sendData();
}



