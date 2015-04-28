#include <Chirp.h>

class LEDExtension : public IExtension
{
public:
	
	virtual void init();
	virtual void update();
	virtual String name();
	LEDExtension();
	LEDExtension(uint8_t frontAddr, uint8_t backAddr);
	void setLED(uint8_t ledN, bool state);
	void setLED(leds ledN, bool state);
	void turnOffLEDs();

	void blink(const unsigned long interval);
	void flowForward(const unsigned long interval, const unsigned short dir);
	void arrow(const unsigned short dir);
	void rotating(const bool forward, const short nrLEDs, const int interval);

private:
	uint8_t ledFrontAddr;
	uint8_t ledBackAddr;
	uint8_t i2cCommand[3];
	uint8_t ledBitMask;
	bool send;
};