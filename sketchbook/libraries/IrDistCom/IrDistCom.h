/* 	
	Chirp Ir communication and distance library v0.1
	by Christian Skjetne
*/

#include <Wire.h>
#include "Arduino.h"

struct sensor_data_t{
  unsigned sensor0 : 10;
  unsigned sensor1 : 10;
  unsigned sensor2 : 10;
  unsigned sensor3 : 10;
};

class IrDistCom
{
	public:
		IrDistCom();
		void begin(uint8_t addrF, uint8_t addrB);
		void getDistSensors(unsigned short distSensors[8]);
		uint8_t sendIrDataBlocking(uint8_t data[], int sizeOfData);//0=f-timeout&b-timeout 1=f-success&b-timeout 2=f-timeout&b-success 3=both-success, 5=Error!
		/*void sendIrDataNonLocking(uint8_t data[32], int sizeOfData);// remember to use the getDataSuccess after this and run it until it returns something else than 5. 
		uint8_t getDataSuccess(); //0=f-timeout&b-timeout 1=f-success&b-timeout 2=f-timeout&b-success 3=both-success, 5=stillWaitingForTransfer-TryAgain */
		uint8_t isIrDataAvailable(); //0=nodata 1=f-data&b-nodata 2=f-nodata&b-data 3=both-data
		uint8_t getIrData(bool frontATtinyHasData, uint8_t dataBuffer[32]); //frontATtinyHasData<-1=frontData/0=backdata(use isIrDataAvailable to find out)
	private:
		static uint8_t irdistcom_addr_front;
		static uint8_t irdistcom_addr_back;
		sensor_data_t sensor_data1, sensor_data2;
};
