#pragma once
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;
using namespace cv;

class Led
{
public:
	// types
	static const int RED=0, GREEN=1, BLUE=2, CALIBRATION=3;

	Led();
	~Led(void);

	Led(int type);

	Led(const Led &led);

	Point2f getPoint();
	void setPoint(Point2f point);

	Scalar getHSVmin();
	Scalar getHSVmax();

	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

	int getType(){ return type; }
	void setType(int type){ this->type = type; }

	Scalar getColour(){
		return Colour;
	}
	void setColour(Scalar c){

		Colour = c;
	}

private:

	Point2f point;
	int type;
	Scalar HSVmin, HSVmax;
	Scalar Colour;
};
