/* 	
	Chirp Distance Sensor library v0.1
	by Jean-Marc Montanier : 130529
*/

#include <Wire.h>
#include "DistSensors.h"

uint8_t DistSensors::sensor_front_addr = 0xF;
uint8_t DistSensors::sensor_back_addr	= 0xB;

DistSensors::DistSensors()
{
}

void DistSensors::begin(uint8_t front_addr, uint8_t back_addr)
{
	sensor_front_addr = front_addr;
	sensor_back_addr = back_addr;
}

void DistSensors::getDistSensors(unsigned short distSensors[8])
{
	Wire.requestFrom(sensor_front_addr, (uint8_t)5);    // request bytes from slave device, BLOCKING!

	//    Serial1.print("\nwaiting for robot...\n");
	int i = 0;
	uint8_t* data1 = (uint8_t*)&sensor_data1;
	while(Wire.available())    // slave may send less than requested
	{ 
		data1[i++] = Wire.read();
		if(i == 5) break;
	}

	Wire.requestFrom(sensor_back_addr, (uint8_t)5);    // request bytes from slave device, BLOCKING!

	int j = 0;
	uint8_t* data2 = (uint8_t*)&sensor_data2;
	while(Wire.available())    // slave may send less than requested
	{ 
		data2[j++] = Wire.read();
		if(j == 5) break;
	}

   distSensors[0] = sensor_data1.sensor0;
   distSensors[1] = sensor_data1.sensor1;
   distSensors[2] = sensor_data1.sensor2;
   distSensors[3] = sensor_data1.sensor3;
   distSensors[4] = sensor_data2.sensor0;
   distSensors[5] = sensor_data2.sensor1;
   distSensors[6] = sensor_data2.sensor2;
   distSensors[7] = sensor_data2.sensor3;
}
