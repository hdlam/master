/* 	
	Chirp LED shield library v0.1
	by Christian Skjetne
*/

#include <Wire.h>
#include "Led_IR.h"

uint8_t Led_IR::led_front_addr = 2;
uint8_t Led_IR::led_back_addr	= 3;
uint8_t Led_IR::i2cCommand[] = {0x00,0x00,0x00};

Led_IR::Led_IR()
{
}

void Led_IR::begin(uint8_t front_addr, uint8_t back_addr)
{
	led_front_addr 	= front_addr;
	led_back_addr	= back_addr;
}

void Led_IR::setLEDs(uint8_t bitmask)
{
	i2cCommand[0] = 'd';

	i2cCommand[1] = bitmask & 0x0f;//B00001111;
    Wire.beginTransmission(led_back_addr); // transmit to device 
    Wire.write(i2cCommand, 3);        // sends bytes
    Wire.endTransmission();    // stop transmitting
	
	i2cCommand[1] = bitmask >> 4;
	Wire.beginTransmission(led_front_addr); // transmit to device 
    Wire.write(i2cCommand, 3);        // sends bytes
    Wire.endTransmission();    // stop transmitting
}

void Led_IR::setLED(uint8_t led, bool enable)
{
	i2cCommand[0] = 'c';
	i2cCommand[2] = 0x01 & enable;

	if(led < 4)
    {
		i2cCommand[1] = led;
		Wire.beginTransmission(led_front_addr); // transmit to device 
		Wire.write(i2cCommand, 3);        // sends bytes
		Wire.endTransmission();    // stop transmitting
    }
    else
    {
		i2cCommand[1] = led-4;//the attiny numbers the led from 0-4
		Wire.beginTransmission(led_back_addr); // transmit to device 
		Wire.write(i2cCommand, 3);        // sends bytes
		Wire.endTransmission();    // stop transmitting
    }
}

void Led_IR::turnOffLEDs()
{
	setLEDs(0x0f);
}