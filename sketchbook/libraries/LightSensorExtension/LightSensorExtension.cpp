#include <Arduino.h>
#include <Chirp.h>
#include "LightSensorExtension.h"

/*uint8_t LightSensorExtension::lightFrontAddr = 2;
uint8_t LightSensorExtension::lightBackAddr	= 3;
uint8_t LightSensorExtension::i2cCommand[] = {0x00,0x00,0x00};
unsigned short LightSensorExtension::sensorData[8];
sensor_data_t LightSensorExtension::sensor_data1;
sensor_data_t LightSensorExtension::sensor_data2;*/

LightSensorExtension::LightSensorExtension() 
{ 
	lightFrontAddr = 2;
	lightBackAddr = 3;
}

LightSensorExtension::LightSensorExtension(uint8_t frontAddr, uint8_t backAddr) 
{ 
	lightFrontAddr = frontAddr;
	lightBackAddr = backAddr;
}

void LightSensorExtension::init() { }

void LightSensorExtension::update()
{
	Wire.requestFrom(lightFrontAddr, (uint8_t)5);    // request bytes from slave device, BLOCKING!

	//    Serial1.print("\nwaiting for robot...\n");
	int i = 0;
	uint8_t* data1 = (uint8_t*)&sensor_data1;
	while(Wire.available())    // slave may send less than requested
	{ 
		data1[i++] = Wire.read();
		if(i == 5) break;
	}

	Wire.requestFrom(lightBackAddr, (uint8_t)5);    // request bytes from slave device, BLOCKING!

	int j = 0;
	uint8_t* data2 = (uint8_t*)&sensor_data2;
	while(Wire.available())    // slave may send less than requested
	{ 
		data2[j++] = Wire.read();
		if(j == 5) break;
	}

	sensorData[0] = sensor_data1.sensor0;
	sensorData[1] = sensor_data1.sensor1;
	sensorData[2] = sensor_data1.sensor2;
	sensorData[3] = sensor_data1.sensor3;
	sensorData[4] = sensor_data2.sensor0;
	sensorData[5] = sensor_data2.sensor1;
	sensorData[6] = sensor_data2.sensor2;
	sensorData[7] = sensor_data2.sensor3;

	//Serial.println("updating lights");
}

String LightSensorExtension::name()
{
	return String("lightSensors");
}

unsigned short* LightSensorExtension::getSensorData()
{
	return sensorData;
}

unsigned short LightSensorExtension::getSensor(uint8_t sensor)
{
	return sensorData[sensor];
}

unsigned short LightSensorExtension::getSensor(leds sensor)
{
	return sensorData[sensor];
}