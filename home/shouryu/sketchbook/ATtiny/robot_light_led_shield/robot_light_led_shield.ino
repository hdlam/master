#include "TinyWireS84.h"

#define led_pin0 7
#define led_pin1 8
#define led_pin2 9
#define led_pin3 10

#define detector_pin0 0
#define detector_pin1 1
#define detector_pin2 2
#define detector_pin3 3

#define i2c_address 0x2 //2 for right ATtiny, 3 for left ATtiny

struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
} sensor_data, preval;

byte i2cCommand[] = {
  0x00,0x00,0x00};
int ledStatus[4];

void setup()
{
  pinMode(led_pin0, OUTPUT);
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(led_pin3, OUTPUT);
  
  TinyWireS.begin(i2c_address);
  TinyWireS.onRequest(on_request);
}

void writeLEDs(byte LEDmap)
{
  int i = 7;
  byte mask;
  for(mask = B0001; mask <= 8; mask <<= 1)
  {
    if(mask & LEDmap)
    {
      ledStatus[i-7] = 1;
        digitalWrite(i++, HIGH);
    }
    else
    {
      ledStatus[i-7] = 0;
        digitalWrite(i++, LOW);
    }
      
  }
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
void readSensors(void)
{
  
  digitalWrite(led_pin0, LOW);
  digitalWrite(led_pin1, LOW);
  digitalWrite(led_pin2, LOW);
  digitalWrite(led_pin3, LOW);

  delay(2000);
  
  sensor_data.sensor0 = analogRead(detector_pin0);
  sensor_data.sensor1 = analogRead(detector_pin1);
  sensor_data.sensor2 = analogRead(detector_pin2);
  sensor_data.sensor3 = analogRead(detector_pin3);
  
  if(ledStatus[0])
    digitalWrite(led_pin0, HIGH);
  if(ledStatus[1])
    digitalWrite(led_pin1, HIGH);
  if(ledStatus[2])
    digitalWrite(led_pin2, HIGH);
  if(ledStatus[3])
    digitalWrite(led_pin3, HIGH);
}

void loop()
{
  //GET DATA FROM MASTER:
  int recByte = 0;
  if(TinyWireS.available() )//we expect only 3 bytes
  {
    while(recByte < 3)
    {
      i2cCommand[recByte] = TinyWireS.receive();
      recByte++;
    }
  }
  switch (i2cCommand[0]) // Command byte
  {
    case 'd':// set all led according to bit mask
      writeLEDs(i2cCommand[1]);
      break;
    case 'c':// set spesific led
      digitalWrite(i2cCommand[1]+7, i2cCommand[2] & B00000001);
      break;
  }
  i2cCommand[0] = 0x00;     //flush buffer
}

void on_request(void)
{
  readSensors();
  sendData();
}



