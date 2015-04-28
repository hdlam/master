/* 	
	Chirp Distance Sensor library v0.1
	by Jean-Marc Montanier : 130529
*/

#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>



struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
};


class Sensors
{
	public:
		Sensors();
		void begin(uint8_t front_addr, uint8_t back_addr); 	//(optional) sets the led i2c addresses. Default is 0xF & 0xB.
		void getSensors(unsigned short distSensors[8], unsigned short IRSensors[8]);
		void getIRSensors(unsigned short IRSensors[8]);
		void getDistanceSensors(unsigned short distSensors[8]);
		void setSmoothing(uint8_t value);

	private:
		void request();
		static uint8_t sensor_front_addr;
		static uint8_t sensor_back_addr;
		sensor_data_t sensor_data1, sensor_data2, IR_data1, IR_data2;
};

#endif