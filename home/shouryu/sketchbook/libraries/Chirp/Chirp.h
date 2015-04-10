/*
	Chirp 
	Håvard Schei 130626
*/
#ifndef CHIRP_H
#define CHIRP_H
#include <Arduino.h>
#include "utility/Sensors.h"
#include "utility/Motors.h"
#include "utility/Consts.h"
#include "utility/extensions/IExtension.h"
#include "utility/Vector.h"
//#include "utility/extensions/Led.h"

enum leds 
{
	FRONT,
	FRONT_RIGHT,
	RIGHT,
	BACK_RIGHT,
	BACK,
	BACK_LEFT,
	LEFT,
	FRONT_LEFT
};

class Chirp
{
	public:
		Chirp();
		~Chirp();
		void init();
		void update();
		void setLeftSpeed(float lspeed);
		void setRightSpeed(float rspeed);
		void setSpeed(float lspeed, float rspeed);
		void setAcceleration(int acc);
		float getLeftSpeed();
		float getRightSpeed();
		unsigned short* getLightSensorData();
		unsigned short* getDistanceSensorData();
		int getLightSensor(leds);
		int getLightSensor(uint8_t);
		int getDistanceSensor(leds);
		int getDistanceSensor(uint8_t);
		void setSensorSmoothing(uint8_t value);
		unsigned long getTime();
		unsigned long getPreviousTime();
		unsigned long getEvaluationTime();
		void timedDriveStraight(const unsigned short time, const float speed);
		void timedDrive(const unsigned short time, const float leftSpeed, const float rightSpeed);
		void moveSteps(int steps, bool block);
		void moveSteps(int steps, int speed, bool block);
		void moveSteps(int steps, int lspeed, int rspeed, bool block);
		void turnDegrees(int degrees, int speed, bool block);
		void turnDirection(leds direction, int speed, bool block);
		void moveCM(int cm, int speed, bool block);
		int getRemainingSteps();
		void addExtension(IExtension* extensions);
		IExtension* getExtension(const char* name);
	private:
		void initExtensions();
		void blockWhileStepping();
		void updateData();
		void updateExtensions();
		void sendData();

		float leftSpeed;
		float rightSpeed;
		int setSteps;
		int stepsLeft;
		bool send;
		Motors motors;
		Sensors sensors;
		unsigned short sensorData[8];
		unsigned short lightData[8];
		unsigned long curTime;
		unsigned long prevTime;
		uint8_t extensionsCount;
		Vector<IExtension* > extensions;
};	

#endif