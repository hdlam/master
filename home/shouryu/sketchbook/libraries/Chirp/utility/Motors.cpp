/* 	
	Chirp Motors library v0.1
	by Jean-Marc Montanier : 130529
*/

#include <Arduino.h>
#include <Wire.h>
#include "Motors.h"
#include "Consts.h"

uint8_t Motors::motor_addr = 1;

/**
Constructor. Resets the i2cCommand.
*/
Motors::Motors()
{
	i2cCommand[0] = 0x00;
	i2cCommand[1] = 0x00;
	i2cCommand[2] = 0x00;
}

/**
Sets the motor address.

@param addr The new address.
*/
void Motors::begin(uint8_t addr)
{
	motor_addr = addr;
}

/**
Sends a command that sets the speeds for both motors in the motor driver. 
If the robot is not already moving, you need to call setMove to move at the given speed indefinitely, or startStepping to move a given number of steps

@param left The left wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
@param right The right wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
*/
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

/**
Sets both wheel speeds, then starts moving.

@param left The left wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
@param right The right wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
*/
void Motors::moveAtSpeeds(int left, int right)
{
	setSpeeds(left,right);
	setMove(true);
}

/**
Sends a command that sets the speed for the left motor in the motor driver. 
If the robot is not already moving, you need to call setMove to move at the given speed indefinitely, or startStepping to move a given number of steps

@param speed The left wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
*/
void Motors::setLeftSpeed(int speed)
{
	short int b1 = (speed >> 8) & 0xFF;
	short int b2 = (speed) & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('L',b1,b2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

/**
Sends a command that sets the speed for the right motor in the motor driver. 
If the robot is not already moving, you need to call setMove to move at the given speed indefinitely, or startStepping to move a given number of steps

@param speed The right wheel speed in the range [-CHIRP_MAXSPEED, CHIRP_MAXSPEED]
*/
void Motors::setRightSpeed(int speed)
{
	short int b1 = (speed >> 8) & 0xFF;
	short int b2 = (speed) & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('R',b1,b2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

/**
Sends a command to the motor driver that sets the amount of acceleration used when using stepping. Deafult is 1000.

@param acc The new acceleration value
*/
void Motors::setAcceleration(int acc)
{
	short int a1 = (acc >> 8) & 0xFF;
	short int a2 = acc & 0xFF;

	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('A',a1,a2), 3);        // sends bytes
	Wire.endTransmission();    // stop transmitting
}

/**
Sends a command to the motor driver that (if set to true) makes the robot move at the set speed. If set to false, the robot will stop moving

@param status True to start moving, False to stop moving
*/
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

/**
Starts moving the given number of steps. Moves at the currently set wheel speeds.

@param lsteps The number of steps to move the left motor
@param rsteps The number of steps to move the right motor
*/
void Motors::startStepping(int lsteps, int rsteps)
{
	short int ls1 = (lsteps >> 8) & 0xFF;
	short int ls2 = lsteps & 0xFF;
	short int rs1 = (rsteps >> 8) & 0xFF;
	short int rs2 = rsteps & 0xFF;
	Wire.beginTransmission(motor_addr); // transmit to device 
	Wire.write(makeCommand('l', ls1, ls2), 3);
	Wire.write(makeCommand('r', rs1, rs2), 3);
	Wire.write(makeCommand('S', 0x00, 0x00), 3);
	Wire.endTransmission();
	
}

/**
Starts moving the given number of steps. Moves at the currently set wheel speeds.

@param steps The number of steps to move both motors
*/
void Motors::moveSteps(int steps)
{
	//setSpeeds(CHIRP_MAXSPEED, CHIRP_MAXSPEED);
	startStepping(steps, steps);
}

/**
Starts moving the given number of steps. Moves at the given wheel speed.

@param steps The number of steps to move both motors
@param speed The speed at which to move both motors
*/
void Motors::moveSteps(int steps, int speed)
{
	
	moveSteps(steps, steps, speed, speed);
}

/**
Starts moving the given number of steps. Moves at the given wheel speed.

@param steps The number of steps to move both motors
@param lspeed The speed at which to move the left motor
@param rspeed The speed at which to move the right motor
*/
void Motors::moveSteps(int steps, int lspeed, int rspeed)
{
	moveSteps(steps, steps, lspeed, rspeed);
}

/**
Starts moving the given number of steps. Moves at the given wheel speed.

@param lsteps The number of steps to move the left motors
@param rsteps The number of steps to move the right motors
@param lspeed The speed at which to move the left motor
@param rspeed The speed at which to move the right motor
*/
void Motors::moveSteps(int lsteps, int rsteps, int lspeed, int rspeed) 
{
	setSpeeds(lspeed, rspeed);
	startStepping(lsteps, rsteps);
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
/**
Extract the int value of the data field from the given buffer.

@param b The byte buffer
*/
int byteBufferToInt16(byte b[])
{
  int i  = b[1] << 8;
  i     |= b[2];
  return i;
}
/**
Requests the number of steps left in the current movement from the motor driver, and returns it.

@return The number of steps left in the motor with the most steps left
*/
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
	return min(byteBufferToInt16(left), byteBufferToInt16(right));

}

/**
Makes a command from three bytes

@param b1 First byte (command)
@param b2 Second byte (data)
@param b3 Third byte (data)

*/

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