/* 	
	Chirp Motors library v0.1
	by Jean-Marc Montanier : 130529
*/

#include <Arduino.h>
#include <Wire.h>
#include "Motors.h"
#include "Consts.h"

uint8_t Motors::motor_addr = 1;


Motors::Motors()
{
	i2cCommand[0] = 0x00;
	i2cCommand[0] = 0x00;
	i2cCommand[0] = 0x00;
}

void Motors::begin(uint8_t addr)
{
	motor_addr = addr;
}

void Motors::setSpeeds(int left, int right)
{
	short int bl1 = (left >> 8) & 0xFF;
	short int bl2 = (left) & 0xFF;

	short int br1 = (right >> 8) & 0xFF;
	short int br2 = (right) & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('L',bl1,bl2), 3);        // sends bytes
	Wire.write(makeCommand('R',br1,br2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

void Motors::moveAtSpeeds(int left, int right)
{
	setSpeeds(left,right);
	setMove(true);
}

void Motors::setLeftSpeed(int speed)
{
	short int b1 = (speed >> 8) & 0xFF;
	short int b2 = (speed) & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('L',b1,b2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

void Motors::setRightSpeed(int speed)
{
	short int b1 = (speed >> 8) & 0xFF;
	short int b2 = (speed) & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('R',b1,b2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

void Motors::setAcceleration(int acc)
{
	short int a1 = (acc >> 8) & 0xFF;
	short int a2 = acc & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('A',a1,a2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

void Motors::setMove(bool status)
{
	if (status == true)
	{
		Wire.beginTransmission(motor_addr); // transmit to device 
		Wire.write(makeCommand('G',0x00,0x00), 3);        // sends bytes
		Wire.endTransmission();    // stop transmitting
	}
	else
	{
		Wire.beginTransmission(motor_addr); // transmit to device 
		Wire.write(makeCommand('H',0x00,0x00), 3);        // sends bytes
		Wire.endTransmission();    // stop transmitting
	}
}

void Motors::startStepping(int lsteps, int rsteps)
{
	short int ls1 = (rsteps >> 8) & 0xFF;
	short int ls2 = rsteps & 0xFF;
	short int rs1 = (rsteps >> 8) & 0xFF;
	short int rs2 = rsteps & 0xFF;
	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('l', ls1, ls2), 3);
	Wire.write(makeCommand('r', rs1, rs2), 3);
	Wire.write(makeCommand('S', 0x00, 0x00), 3);
	Wire.endTransmission();
	
}

void Motors::moveSteps(int steps)
{
	//setSpeeds(CHIRP_MAXSPEED, CHIRP_MAXSPEED);
	startStepping(steps, steps);
}

void Motors::moveSteps(int steps, int speed)
{
	setSpeeds(speed, speed);
	moveSteps(steps);
}

void Motors::moveSteps(int steps, int lspeed, int rspeed)
{
	setSpeeds(lspeed, rspeed);
	moveSteps(steps);
}

void Motors::moveSteps(int lsteps, int rsteps, int lspeed, int rspeed) 
{
	setSpeeds(lspeed, rspeed);
	startStepping(lspeed, rspeed);
}

void Motors::turnDegrees(int degrees, int speed)
{
	int dir = 1;
	if(degrees < 0) {
		degrees *= -1;
		dir = -1;
	}
	float revs_per_turn = ROBOT_CIRC/(float)WHEEL_CIRC;
	moveSteps(degrees / 360.0f * revs_per_turn * STEPS_PER_REV, dir*speed, -dir*speed);
}

void Motors::turnDegrees(int degrees)
{
	turnDegrees(degrees, 400);
}
// extract the int value of the data field from the given buffer.
int byteBufferToInt16(byte b[])
{
  int i  = b[1] << 8;
  i     |= b[2];
  return i;
}

int Motors::stepsLeft()
{
	Wire.requestFrom(motor_addr, (uint8_t)6);    // request bytes from slave device, BLOCKING!

	uint8_t left[3];
	uint8_t right[3];

	//    Serial1.print("\nwaiting for robot...\n");
	int i = 0;
	while(Wire.available())    // slave may send less than requested
	{ 
		left[i++] = Wire.read();
		if(i == 3) break;
	}

	i = 0;
	while(Wire.available())    // slave may send less than requested
	{ 
		right[i++] = Wire.read();
		if(i == 3) break;
	}
	return max(byteBufferToInt16(left), byteBufferToInt16(right));

}

uint8_t* Motors::makeCommand(uint8_t b1, uint8_t b2, uint8_t b3)
{
	i2cCommand[0] = b1;
	i2cCommand[1] = b2;
	i2cCommand[2] = b3;

	return i2cCommand;
}

float Motors::getStepsPerCM()
{
	return STEPS_PER_CM;
}