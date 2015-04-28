/* 	
		Chirp Motors library v0.1
		by Jean-Marc Montanier : 130529
 */
#ifndef Motors_guard

#include <Wire.h>

#define Motors_guard

class Motors
{

	public:
		Motors();
		void begin(uint8_t addr); 	//(optional) sets the led i2c addresses. Default is 1.

		void moveAtSpeeds(int left, int right);

		void setSpeeds(int left, int right);
		void setLeftSpeed(int speed);
		void setRightSpeed(int speed);
		void setAcceleration(int acc);
		void setMove(bool status);

		int stepsLeft();
		void startStepping(int, int);
		void moveSteps(int);
		void moveSteps(int, int);
		void moveSteps(int, int, int);
		void moveSteps(int, int, int, int);
		void turnDegrees(int);
		void turnDegrees(int, int);
		float getStepsPerCM();

	private:
		static uint8_t motor_addr;
		uint8_t* makeCommand(uint8_t b1, uint8_t b2, uint8_t b3);
		uint8_t i2cCommand[3];
};

#endif