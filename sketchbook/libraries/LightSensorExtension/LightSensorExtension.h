#include <Chirp.h>


class LightSensorExtension : public IExtension
{
public:
	virtual void init();
	virtual void update();
	virtual String name();
	
	LightSensorExtension();
	LightSensorExtension(uint8_t frontAddr, uint8_t backAddr);

	unsigned short* getSensorData();
	unsigned short getSensor(uint8_t sensor);
	unsigned short getSensor(leds sensor);

private:
	uint8_t lightFrontAddr;
	uint8_t lightBackAddr;
	uint8_t i2cCommand[3];
	unsigned short sensorData[8];
	sensor_data_t sensor_data1;
	sensor_data_t sensor_data2;

};