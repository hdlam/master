/* 	
	Chirp Distance Sensor library v0.1
	by Jean-Marc Montanier : 130529
*/

#include <Wire.h>
#include "Sensors.h"

uint8_t Sensors::sensor_front_addr = 0xF;
uint8_t Sensors::sensor_back_addr	= 0xB;

Sensors::Sensors()
{
}
/**
Sets the addresses of the two sensor controllers
*/
void Sensors::begin(uint8_t front_addr, uint8_t back_addr)
{
	sensor_front_addr = front_addr;
	sensor_back_addr = back_addr;
}

/**
Requests and reads the sensor data received from the sensor controllers
*/
void Sensors::request()
{
	Wire.requestFrom(sensor_front_addr, (uint8_t)10);    // request bytes from slave device, BLOCKING!

	//    Serial1.print("\nwaiting for robot...\n");
	int i = 0;
	uint8_t* data1 = (uint8_t*)&sensor_data1;
	while(Wire.available())    // slave may send less than requested
	{ 
		data1[i++] = Wire.read();
		if(i == 5) break;
	}
	i = 0;
	uint8_t* light1 = (uint8_t*)&IR_data1;
	while(Wire.available())    // slave may send less than requested
	{ 
		light1[i++] = Wire.read();
		if(i == 5) break;
	}


	Wire.requestFrom(sensor_back_addr, (uint8_t)10);    // request bytes from slave device, BLOCKING!

	int j = 0;
	uint8_t* data2 = (uint8_t*)&sensor_data2;
	while(Wire.available())    // slave may send less than requested
	{ 
		data2[j++] = Wire.read();
		if(j == 5) break;
	}
	j = 0;
	uint8_t* light2 = (uint8_t*)&IR_data2;
	while(Wire.available())    // slave may send less than requested
	{ 
		light2[j++] = Wire.read();
		if(j == 5) break;
	}
}

/**
Updates the distSensors and IRSensors arrays with new sensor values

@param distSensors The array of distance sensor data
@param IRSensors The array of IR sensor data
*/
void Sensors::getSensors(unsigned short distSensors[8], unsigned short IRSensors[8])
{	
	request();
	distSensors[0] = sensor_data1.sensor0;
	distSensors[1] = sensor_data1.sensor1;
	distSensors[2] = sensor_data1.sensor2;
	distSensors[3] = sensor_data1.sensor3;
	distSensors[4] = sensor_data2.sensor0;
	distSensors[5] = sensor_data2.sensor1;
	distSensors[6] = sensor_data2.sensor2;
	distSensors[7] = sensor_data2.sensor3;

	IRSensors[0] = IR_data1.sensor0;
	IRSensors[1] = IR_data1.sensor1;
	IRSensors[2] = IR_data1.sensor2;
	IRSensors[3] = IR_data1.sensor3;
	IRSensors[4] = IR_data2.sensor0;
	IRSensors[5] = IR_data2.sensor1;
	IRSensors[6] = IR_data2.sensor2;
	IRSensors[7] = IR_data2.sensor3;
}

/**
Updates the distSensors array with new sensor values

@param distSensors The array of distance sensor data
*/
void Sensors::getDistanceSensors(unsigned short distSensors[8])
{
	request();
	distSensors[0] = sensor_data1.sensor0;
	distSensors[1] = sensor_data1.sensor1;
	distSensors[2] = sensor_data1.sensor2;
	distSensors[3] = sensor_data1.sensor3;
	distSensors[4] = sensor_data2.sensor0;
	distSensors[5] = sensor_data2.sensor1;
	distSensors[6] = sensor_data2.sensor2;
	distSensors[7] = sensor_data2.sensor3;
}

/**
Updates the IRSensors array with new sensor values

@param IRSensors The array of IR sensor data
*/
void Sensors::getIRSensors(unsigned short IRSensors[8])
{
	request();
	IRSensors[0] = IR_data1.sensor0;
	IRSensors[1] = IR_data1.sensor1;
	IRSensors[2] = IR_data1.sensor2;
	IRSensors[3] = IR_data1.sensor3;
	IRSensors[4] = IR_data2.sensor0;
	IRSensors[5] = IR_data2.sensor1;
	IRSensors[6] = IR_data2.sensor2;
	IRSensors[7] = IR_data2.sensor3;
}

void Sensors::setSmoothing(uint8_t value) 
{
	Wire.beginTransmission(sensor_back_addr);
	Wire.write(value);
	Wire.endTransmission();

	Wire.beginTransmission(sensor_front_addr);
	Wire.write(value);
	Wire.endTransmission();
}
