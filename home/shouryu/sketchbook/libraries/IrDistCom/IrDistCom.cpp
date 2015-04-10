/* 	
	Chirp Ir communication and distance library v0.1
	by Christian Skjetne
*/

#include <Wire.h>
#include "Arduino.h"
#include "IrDistCom.h" 

#define front_com_pin 4
#define back_com_pin 5

uint8_t IrDistCom::irdistcom_addr_front = 0xF;
uint8_t IrDistCom::irdistcom_addr_back = 0xB;

IrDistCom::IrDistCom()
{
}

void IrDistCom::begin(uint8_t addrF, uint8_t addrB)
{
	irdistcom_addr_front	= addrF;
	irdistcom_addr_back		= addrB;
}

void IrDistCom::getDistSensors(unsigned short distSensors[8])
{
	Wire.beginTransmission(irdistcom_addr_front);
    Wire.write('D');
    Wire.write(0);
    Wire.endTransmission();

	Wire.requestFrom(irdistcom_addr_front, (uint8_t)5);    // request uint8_ts from slave device, BLOCKING!

	//    Serial.print("\nwaiting for robot...\n");
	int i = 0;
	uint8_t* data1 = (uint8_t*)&sensor_data1;
	while(Wire.available())    // slave may send less than requested
	{ 
		data1[i++] = Wire.read();
		if(i == 5) break;
	}

	Wire.beginTransmission(irdistcom_addr_back);
    Wire.write('D');
    Wire.write(0);
    Wire.endTransmission();
	
	Wire.requestFrom(irdistcom_addr_back, (uint8_t)5);    // request uint8_ts from slave device, BLOCKING!

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

uint8_t IrDistCom::sendIrDataBlocking(uint8_t data[], int sizeOfData) //max data size is 32 uint8_ts
{
	//send data to the front ATtiny:
	Wire.beginTransmission(irdistcom_addr_front);
    Wire.write('S');
    Wire.write(sizeOfData);
    for(int i = 0; i < sizeOfData; i++)
		Wire.write(data[i]);      
    Wire.endTransmission();
	
	//data sent to the front ATtiny, lets do the back:
	Wire.beginTransmission(irdistcom_addr_back);
    Wire.write('S');
    Wire.write(sizeOfData);
    for(int i = 0; i < sizeOfData; i++)
		Wire.write(data[i]);      
    Wire.endTransmission();
	
	//this is the locking method, do a spinlock to wait for the transmissions to either succeed, or time out.
	delay(1); // give the ATtiny time to react. TODO: replace with interrupt.
	while(true)
	{
		if(digitalRead(front_com_pin) == LOW && digitalRead(back_com_pin) == LOW) //both ATtinies have ended the transmission.
		{
			//time to get the results.
			Wire.requestFrom(irdistcom_addr_front, (uint8_t)1);
			int success1 = -2;
			while(Wire.available())    // slave may send less than requested
			{
				success1 = Wire.read();
				break;
			}
			
			Wire.requestFrom(irdistcom_addr_back, (uint8_t)1);
			int success2 = -2;
			while(Wire.available())    // slave may send less than requested
			{
				success2 = Wire.read();
				break;
			}
			
			if(success1 == 0 && success2 == 0)//both timed out
			{
				return 0;
			}
			else if(success1 == 1 && success2 == 0)//front succeeded, back timed out
			{
				return 1;
			}
			else if(success1 == 0 && success2 == 1)//front timed out, back succeeded
			{
				return 2;
			}
			else if(success1 == 1 && success2 == 1)//both succeeded
			{
				return 3;
			}
			else //illegal state, something went wrong
				return 4;
		}
		delay(10);//wait for the ATtinies to end transmission.
	}
}
/*
void IrDistCom::sendIrDataNonLocking(uint8_t data[32], int sizeOfData) //max data size is 32 uint8_ts
{
	//send data to the front ATtiny:
	Wire.beginTransmission(irdistcom_addr_front);
    Wire.write('S');
    Wire.write(sizeOfData);
    for(int i = 0; i < sizeOfData; i++)
		Wire.write(data[i]);      
    Wire.endTransmission();
	
	//data sent to the front ATtiny, lets do the back:
	Wire.beginTransmission(irdistcom_addr_back);
    Wire.write('S');
    Wire.write(sizeOfData);
    for(int i = 0; i < sizeOfData; i++)
		Wire.write(data[i]);      
    Wire.endTransmission();
}

uint8_t IrDistCom::getDataSuccess()//TODO: replace with interrupt.
{
	//this is the non locking method, check the transmissions to see if they either succeed, or timed out.

	if(digitalRead(front_com_pin) == LOW && digitalRead(back_com_pin) == LOW) //both ATtinies have ended the transmission.
	{
		//time to get the results.
		Wire.requestFrom(irdistcom_addr_front, (uint8_t)1);
		int success1 = -2;
		while(Wire.available())    // slave may send less than requested
		{
			success1 = Wire.read();
			break;
		}
		
		Wire.requestFrom(irdistcom_addr_back, (uint8_t)1);
		int success2 = -2;
		while(Wire.available())    // slave may send less than requested
		{
			success2 = Wire.read();
			break;
		}
		
		if(success1 == 0 && success2 == 0)//both timed out
		{
			return 0;
		}
		else if(success1 == 1 && success2 == 0)//front succeeded, back timed out
		{
			return 1;
		}
		else if(success1 == 0 && success2 == 1)//front timed out, back succeeded
		{
			return 2;
		}
		else if(success1 == 1 && success2 == 1)//both succeeded
		{
			return 3;
		}
		
	}
	else //The attinies still have their status pins high. wait for time out or success.
		return 5;
}*/

uint8_t IrDistCom::isIrDataAvailable()
{
	bool frontStatus = digitalRead(front_com_pin);
	bool backStatus	= digitalRead(back_com_pin);

	if(frontStatus == 0 && backStatus == 0)
	{
		return 0;//no data
	}
	else if(frontStatus == 1 && backStatus == 0)
	{
		return 1;//front has data
	}
	else if(frontStatus == 0 && backStatus == 1)
	{
		return 2;//back has data
	}
	else if(frontStatus == 1 && backStatus == 1)
	{
		return 3;//both have data
	}
}

uint8_t IrDistCom::getIrData(bool frontATtinyHasData, uint8_t dataBuffer[32])
{
	uint8_t addrToData = irdistcom_addr_front;
	if( !frontATtinyHasData)
		addrToData = irdistcom_addr_back;
		
	//send the get data command
    Wire.beginTransmission(addrToData);
    Wire.write('R');
    Wire.write(0);
    Wire.endTransmission();
    
	//get the number of uint8_ts available
    Wire.requestFrom(addrToData, (uint8_t)1);
    uint8_t sizeToRead = 0;
    while(Wire.available())    // slave may send less than requested
    {
      sizeToRead = Wire.read();
    }

    if (sizeToRead != 0)//no data? ok.
    {
      Wire.requestFrom(addrToData, (uint8_t)sizeToRead);
      int i = 0;
      while(Wire.available())    // slave may send less than requested
      { 
        dataBuffer[i++] = Wire.read();
        if(i == sizeToRead) break;
      }
    }
	
	return sizeToRead;
}







