#include <TinyWireS84.h>

#define led_pin0 7
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define detector_pin0 0
#define detector_pin1 1
#define detector_pin2 2
#define detector_pin3 3

#define i2c_address 0xB

struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
} sensor_data;

void setup()
{
  pinMode(led_pin0, OUTPUT);
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
  
  TinyWireS.begin(i2c_address);
  TinyWireS.onRequest(on_request);
}

void readSensors(void)
{
  
  int preVal = analogRead(detector_pin0);
  digitalWrite(led_pin0, HIGH);
  delay(1);
  int newVal = analogRead(detector_pin0);
  if(preVal < newVal) 
    sensor_data.sensor0 = newVal-preVal;
  digitalWrite(led_pin0, LOW);

  
  preVal = analogRead(detector_pin1);
  digitalWrite(led_pin1, HIGH);
  delay(1);
  newVal = analogRead(detector_pin1);
  if(preVal < newVal) 
    sensor_data.sensor1 = newVal-preVal;
  digitalWrite(led_pin1, LOW);
  
  preVal = analogRead(detector_pin2);
  digitalWrite(led_pin2, HIGH);
  delay(1);
  newVal = analogRead(detector_pin2);
  if(preVal < newVal) 
    sensor_data.sensor2 = newVal-preVal;
  digitalWrite(led_pin2, LOW);
  
  preVal = analogRead(detector_pin3);
  digitalWrite(led_pin3, HIGH);
  delay(1);
  newVal = analogRead(detector_pin3);
  if(preVal < newVal) 
    sensor_data.sensor3 = newVal-preVal;
  digitalWrite(led_pin3, LOW);
}

void sendData(void)
{  
  uint8_t* data = (uint8_t*)&sensor_data;
  TinyWireS.send(data[0]);
  TinyWireS.send(data[1]);
  TinyWireS.send(data[2]);
  TinyWireS.send(data[3]);
  TinyWireS.send(data[4]);
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



