#include "Led.h"

Led::Led()
{
	//set values for default constructor
	setType(-1);
	setColour(Scalar(0, 0, 0));

}

Led::Led(int type){

	setType(type);

	if (type == BLUE){

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for blue:
		setColour(Scalar(255, 0, 0));

	}
	if (type == RED){

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for red:
		setColour(Scalar(0, 0, 255));

	}
	if (type == GREEN){

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for green:
		setColour(Scalar(0, 255, 0));
	}
	if (type == CALIBRATION){

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for green:
		setColour(Scalar(255,255,255));
	}
}

Led::Led(const Led &led) {
	point = Point2f(led.point);
	type = led.type;
	HSVmin = led.HSVmin;
	HSVmax = led.HSVmax;
	Colour = led.Colour;
}

Led::~Led(void) {
}


Point2f Led::getPoint() {

	return Led::point;

}

void Led::setPoint(Point2f point){

	Led::point = point;

}

Scalar Led::getHSVmin(){

	return Led::HSVmin;

}
Scalar Led::getHSVmax(){

	return Led::HSVmax;
}

void Led::setHSVmin(Scalar min){

	Led::HSVmin = min;
}


void Led::setHSVmax(Scalar max){

	Led::HSVmax = max;
}
