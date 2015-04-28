#include <Arduino.h>
#include <Chirp.h>
#include "LEDExtension.h"

/**
Creates a new instance of the LEDExtension.
*/
LEDExtension::LEDExtension() 
{ 
	ledFrontAddr = 2;
	ledBackAddr = 3;
}

/**
Creates a new instance of the LEDExtension with the given addresses.

@param The front address.
@param The back address.
*/
LEDExtension::LEDExtension(uint8_t frontAddr, uint8_t backAddr)
{
	ledFrontAddr = frontAddr;
	ledBackAddr = backAddr;
}
/**
Initializes the LED shield extension. Does nothing.
*/
void LEDExtension::init() { }

/**
Returns the name of the extension.
*/
String LEDExtension::name()
{
	return String("led");
}

/**
Sends the set data to the LED shield.
*/
void LEDExtension::update()
{
	if (!send)
		return;
	send = false;

	i2cCommand[0] = 'd';

	i2cCommand[1] = ledBitMask & 0x0f;//B00001111;
    Wire.beginTransmission(ledBackAddr); // transmit to device 
    Wire.write(i2cCommand, 3);        // sends bytes
    Wire.endTransmission();    // stop transmitting
	
	i2cCommand[1] = ledBitMask >> 4;
	Wire.beginTransmission(ledFrontAddr); // transmit to device 
    Wire.write(i2cCommand, 3);        // sends bytes
    Wire.endTransmission();    // stop transmitting

    //Serial.println("Updating leds");
}

/**
Turns the specified LED on or off.

@param ledN The integer value specifying the LED that will change state.
@param state The new value for the specified LED, on or off.

*/
void LEDExtension::setLED(uint8_t ledN, bool state)
{
	ledN = (ledN + 4) % 8;

	if (state)
	{
		bitSet(ledBitMask, ledN);
	}
	else
	{
		bitClear(ledBitMask, ledN);
	}
	send = true;
}

/**
Turns the specified LED on or off.

@param ledN The enum value specifying the LED that will change state.
@param state The new value for the specified LED, on or off.
*/
void LEDExtension::setLED(leds led, bool state)
{
	setLED((uint8_t)led, state);
}

/**
Turns off all the LEDs on the LED shield
*/
void LEDExtension::turnOffLEDs()
{
	ledBitMask = 0;
	send = true;
}
void LEDExtension::blink(const unsigned long interval)
{
	static unsigned long previousTime = 0;
	static bool state = true;
	long curTime = millis();

	if (curTime - previousTime > interval)
	{
		previousTime = curTime;
		state = !state;
	}

	if (!state)
		turnOffLEDs();
}

void LEDExtension::flowForward(const unsigned long interval, const unsigned short dir)
{
	static unsigned int progress = 0;
	static unsigned long previousTime = 0;
	long curTime = millis();

	if (curTime - previousTime > interval)
	{
		previousTime = curTime;
		progress++;
	}

	turnOffLEDs();
	switch (progress % 5)
	{
		case 0:
			setLED(4 + dir % 8, true);
			break;
		case 1:
			setLED(3 + dir % 8, true);
			setLED(5 + dir % 8, true);
			break;
		case 2:
			setLED(2 + dir % 8, true);
			setLED(6 + dir % 8, true);
			break;
		case 3:
			setLED(1 + dir % 8, true);
			setLED(7 + dir % 8, true);
			break;	
		case 4:
			setLED(0 + dir % 8, true);
			break;
	}
}

void LEDExtension::arrow(const unsigned short dir)
{
	turnOffLEDs();
	
	setLED(7 + dir % 8, true);
	setLED(0 + dir % 8, true);
	setLED(1 + dir % 8, true);
	setLED(4 + dir % 8, true);
}

void LEDExtension::rotating(const bool forward, const short nrLEDs, const int interval)
{
	static unsigned long previousTime = 0;
	static unsigned short index = 0;
	long curTime = millis();

	if (curTime - previousTime > interval)
	{
		previousTime = curTime;
		index = ++index % 8;
	}

	turnOffLEDs();

	short dir = forward ? 1 : -1;

	for (int i = 0; i < nrLEDs; i+= dir)
	{
		setLED((index + i) % 8, true);
	}
}
/*
void Led::setLED(uint8_t led,bool enable)
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
}*/

