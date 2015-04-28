/**
\mainpage Chirp Library Documentation

\section intro_sec Introduction

This is the main library used for controlling the ChiRP robot and the only one needed to use the base functionality of the robot.

Take a look at http://chirp.idi.ntnu.no/index.php?n=Main.GetStarted to see the basic structure of a ChIRP program.


*/

#include "Chirp.h"


Chirp::Chirp()
{
	
}

Chirp::~Chirp() { }

/**
Initializes the robot. Sets up the i2c connection to the sensor and motor boards. Must be placed in the setup() function.
*/
void Chirp::init()
{
	// Join i2c bus (address optional for master).
	Wire.begin();
	initExtensions();
}

/**
Retreives the sensor data from the sensor board and makes it available using the getSensordata method.
*/
void Chirp::updateData()
{
	sensors.getSensors(sensorData, lightData);
	prevTime = curTime;
	curTime = millis();
	stepsLeft = motors.stepsLeft();
}

/**
Sends the new wheel speeds to the motor driver.
*/
void Chirp::sendData()
{
	
	if(send)
		send = false;
	else 
		return;
	if(setSteps) 
	{
		int lSteps = setSteps;
		int rSteps = setSteps;
		if(abs(leftSpeed) > abs(rightSpeed))
			rSteps = setSteps * abs(rightSpeed/leftSpeed);
		if(abs(leftSpeed) < abs(rightSpeed))
			lSteps = setSteps * abs(leftSpeed/rightSpeed);
		motors.moveSteps(lSteps, rSteps, CHIRP_MAXSPEED * leftSpeed / 100.0f, CHIRP_MAXSPEED * rightSpeed / 100.0f);
		stepsLeft = setSteps;
		setSteps = 0;
	} 
	else
	{
		motors.moveAtSpeeds(CHIRP_MAXSPEED * leftSpeed / 100.0f, CHIRP_MAXSPEED * rightSpeed / 100.0f);	
	}
}
/**
Retrieves the sensor data and sends the set robot data.
*/
void Chirp::update()
{
	updateExtensions();
	updateData();
	sendData();
}
/**
Initializes all extensions added to the robot.
*/
void Chirp::initExtensions()
{
	for (int i = 0; i < extensions.size(); i++) 
 	{ 
 		extensions[i]->init(); 
 	}
}
/**
Updates all the extensions added to the robot.
*/
void Chirp::updateExtensions()
{
	for (int i = 0; i < extensions.size(); i++) 
 	{ 
 		extensions[i]->update(); 
 	}
}

/**
Sets the left wheel speed to be sent to the motor driver using sendData.

@param speed The speed to set the motor to in the range [100, -100].
*/
void Chirp::setLeftSpeed(float speed)
{
	 leftSpeed = constrain(speed, -100, 100);
	 send = true;
}

/**
Sets the right wheel speed to be sent to the motor driver using sendData.

@param speed The speed to set the motor to in the range [100, -100].
*/
void Chirp::setRightSpeed(float speed)
{
	rightSpeed = constrain(speed, -100, 100);//min(100, max(-100, speed));
	send = true;
}

/**
Sets both wheel speeds to be sent to the motor driver using sendData.

@param left The speed to set the left motor to in the range [100, -100].
@param right The speed to set the right motor to in the range [100, -100].
*/
void Chirp::setSpeed(float left, float right)
{
	setLeftSpeed(left);
	setRightSpeed(right);
}

/**
Sets the acceleraton used when using stepping functions. Default is 1000.

@param acc The new acceleration value
*/
void Chirp::setAcceleration(int acc)
{
	motors.setAcceleration(acc);
}

/**
Retrieves the current speed of the left motor.

@return returns the current left wheel speed in the range [100, -100].
*/
float Chirp::getLeftSpeed()
{
	return leftSpeed;
}

/**
Retrieves the current speed of the right motor.

@return returns the current right wheel speed in the range [100, -100].
*/
float Chirp::getRightSpeed()
{
	return rightSpeed;
}

/**
Returns the last distance sensor data retreived.

@return Array of the 8 sensor values in the range [0, 700], where 0 means nothing detected and 700 is the maximum.
*/
unsigned short* Chirp::getDistanceSensorData()
{
	return sensorData;
}

/**
Returns the last light sensor data retreived.

@return Array of the 8 sensor values in the range [0, 700], where 0 means nothing detected and 700 is the maximum.
*/
unsigned short* Chirp::getLightSensorData()
{
	return lightData;
}

/**
Returns the last distance sensor data retrieved from the specified sensor.

@param sensor Then sensor the value is to be retrieved from Valud parameters are in the range [0, 7], beginning with the front sensor and iterating clockwise.
@return The int value value from the specified sensor, range [0, 700].
*/
int Chirp::getDistanceSensor(uint8_t sensor)
{
	return sensorData[sensor];
}

void Chirp::setSensorSmoothing(uint8_t value) 
{
	sensors.setSmoothing(value);
}

/**
Returns the last light sensor data retrieved from the specified sensor.

@param sensor Then sensor the value is to be retrieved from Valud parameters are in the range [0, 7], beginning with the front sensor and iterating clockwise.
@return The int value value from the specified sensor, range [0, 700].
*/
int Chirp::getLightSensor(uint8_t sensor)
{
	return lightData[sensor];
}

/**
Returns the last distance sensor data retrieved from the specified sensor.

@param sensor Then sensor the value is to be retrieved from Valud parameters are in the range [0, 7], beginning with the front sensor and iterating clockwise.
@return The sensor the value is to be reteived from. Valid parameters are: front, front_right, right, back_right, back, back_left, left, front_left.

*/
int Chirp::getDistanceSensor(leds sensor)
{
	return sensorData[sensor];
}

/**
Returns the last light sensor data retrieved from the specified sensor.

@param sensor Then sensor the value is to be retrieved from Valud parameters are in the range [0, 7], beginning with the front sensor and iterating clockwise.
@return The sensor the value is to be reteived from. Valid parameters are: front, front_right, right, back_right, back, back_left, left, front_left.
doxy
*/
int Chirp::getLightSensor(leds sensor)
{
	return lightData[sensor];
}


/**
Returns the current time passed measured at the last update() call.

@return The time in milliseconds measured at the last update() call.
*/
unsigned long Chirp::getTime()
{
	return curTime;
}
/**
Returns the time measured passed at the previous update call.

@return The time in milliseconds from the previous update() call
*/
unsigned long Chirp::getPreviousTime()
{
	return prevTime;
}

/**
Returns the time between the last two update() calls;

@return The time in milliseconds between the last two update calls.
*/
unsigned long Chirp::getEvaluationTime()
{
	return curTime - prevTime;
}

/**
Makes the robot move x steps at full speed right ahead. Non-blocking if block argument set to false. Use getRemainingSteps() to see how many steps are left if not using blocking.
Does not support negative numbers of steps. To move backwards, use a method here you can set the speed to be negative. 

@param steps The number of step to move. Cannot be negative.
@param block Whether to automatically use blocking
*/
void Chirp::moveSteps(int steps, bool block)
{
	moveSteps(steps, 100, block);
}

/**
Makes the robot move x steps at the given speed. Non-blocking if block argument set to false. Use getRemainingSteps() to see how many steps are left if not using blocking.
Set a negative speed to move backwards.

@param steps The number of step to move. Cannot be negative
@param speed The speed at which to move.
@param block Whether to automatically use blocking
*/
void Chirp::moveSteps(int steps, int speed, bool block)
{
	moveSteps(steps, speed, speed, block);
}

/**
Makes the robot move x steps at the given speeds. Non-blocking if block argument set to false. Use getRemainingSteps() to see how many steps are left if not using blocking.
Set negative speed to move backwards, different speeds to turn, etc. 

@param steps The number of step to move.
@param lspeed The speed at which to move the left motor.
@param rspeed The speed at which to move the right motor.
@param block Whether to automatically use blocking.
*/
void Chirp::moveSteps(int steps, int lspeed, int rspeed, bool block)
{
	setSpeed(lspeed, rspeed);
	setSteps = steps;
	send = true;
	if(block)
		blockWhileStepping();
}

/**
Makes the robot move ROUGHLY the given distance in cm. It is fairly accurate, but annot guarantee the exact length is driven.
speed and direction can be defined by setting the speed.

@param cm The distance to travel in cm. Cannot be negative.
@param speed The sped at which to move. Negative speed makes the robot move backwards.
@param block Makes the call blocking.
*/
void Chirp::moveCM(int cm, int speed, bool block)
{
	moveSteps((int)(cm * STEPS_PER_CM), speed, block);
}

/**
Turns the robot in place ROUGHLY by the given number of degrees. Negative numbers makes it turn the other way.

@param degrees The amount and direction to turn.
@param speed The speed at which to turn.
@param block Whether to make the call blocking. 
*/
void Chirp::turnDegrees(int degrees, int speed, bool block)
{
	int dir = 1;
	if(degrees < 0) {
		degrees *= -1;
		dir = -1;
	}
	float revs_per_turn = ROBOT_CIRC/(float)WHEEL_CIRC;
	moveSteps(degrees / 360.0f * revs_per_turn * STEPS_PER_REV, dir * speed, -dir * speed, block);
}

/**
Turns the robot ROUGHLY in the given direction. 

@param direction The direction in which to turn. Directions are FRONT, FRONT_RIGHT, RIGHT, BACK_RIGHT, BACK, BACK_LEFT, LEFT, FRONT_LEFT.
@param speed The speed at which to turn
@param block Whether to make the call blocking
*/
void Chirp::turnDirection(leds direction, int speed, bool block)
{
	int degrees = 0;
	switch(direction)
	{
		case FRONT:
			break;
		case FRONT_RIGHT:
			degrees = 45;
			break;
		case RIGHT:
			degrees = 90;
			break;
		case BACK_RIGHT:
			degrees = 135;
			break;
		case BACK:
			degrees = 180;
			break;
		case BACK_LEFT:
			degrees = -135;
			break;
		case LEFT:
			degrees = -90;
			break;
		case FRONT_LEFT:
			degrees = -45;
			break;
	}
	turnDegrees(degrees, speed, block);
}

/**
Blocks until motor has completed its stepping.
*/
void Chirp::blockWhileStepping()
{
	do
	{
		update();
		delay(100);
	}
	while(stepsLeft > 0 && leftSpeed != 0 && rightSpeed != 0);
}

/**
Returns the number of steps remaining in the current command, if any stepping command has been given

@return The number of steps remaining.
*/
int Chirp::getRemainingSteps()
{
	return stepsLeft;
}
/**
Add an extensions to the robot. The robot will take care of initializing and updating these extensions, 
and they can be used through the robot.
@param An extensions derived from IExtension to add to the robot.
*/
void Chirp::addExtension(IExtension* extension)
{
	extensions.push_back(extension);
}

/**
Method for getting an IExtension by name.

@param The name of the extension.
@return Returns a pointer to the extensions if found.
*/
IExtension* Chirp::getExtension(const char* name)
{
	String sName = String(name);
	
	for (int i = 0; i < extensions.size(); i++)
	{
		if (sName.equals(extensions[i]->name()))
			return extensions[i];
	}
	return 0;
}
