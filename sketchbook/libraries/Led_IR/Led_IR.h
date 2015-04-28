/* 	
	Chirp LED shield library v0.1
	by Christian Skjetne
*/
#ifndef Led_ir_guard

#include <Wire.h>

#define Led_ir_guard

class Led_IR
{

public:
	
	Led_IR();
	void begin(uint8_t front_addr, uint8_t back_addr); 	//(optional) sets the led i2c addresses. Default is 2 & 3.
	void setLEDs(uint8_t bitmask);						//sets all of the led to match the bit mask
	void setLED(uint8_t led,bool enable);				//sets led at the given position to on or off.
	void turnOffLEDs(); 								//turns off all the leds.		
private:
	static uint8_t led_front_addr;
	static uint8_t led_back_addr;
	static uint8_t i2cCommand[];
};

#endif