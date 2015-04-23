// if the program is crashing when you try to start it, he is loading too many points: go look your HSV treshhold of blue, red and calibration point: line 361

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "Led.h"
#include <valarray>
#include <cmath>
#include "../util/functions.h"
#include "../tracker/Tracker.h"
#include "../util/Config.h"

#include "BoundingBoxHandler.h"
#include "CameraHandler.h"

namespace CameraHandler {

CameraHandler::CameraHandler(int frameWidth, int frameHeight) {
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	maxObjectArea = std::pow(std::min(frameWidth, frameHeight) * util::Config::getCameraLedSize(),2);
}

CameraHandler::~CameraHandler() {
	// TODO Auto-generated destructor stub
}


bool CameraHandler::initializeCapture(int device) {

	capture = new VideoCapture(device);
	if (!capture->isOpened()) {
		cerr << "Could not open camera" << endl;
		return false;
	}
	if(util::Config::getCameraBrightness() >= 0) {
		capture->set(CV_CAP_PROP_BRIGHTNESS, util::Config::getCameraBrightness());
	}
	if(util::Config::getCameraContrast() >= 0) {
		capture->set(CV_CAP_PROP_CONTRAST, util::Config::getCameraContrast());
	}
	if(util::Config::getCameraSaturation() >= 0) {
		capture->set(CV_CAP_PROP_SATURATION, util::Config::getCameraSaturation());
	}

	//set height and width of capture frame
	capture->set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

	return true;

}

vector<Led> CameraHandler::findObjects(Led theLed) {
	threshold(HSV, theLed.getHSVmin(), theLed.getHSVmax(), thresh);
	vector<Led> leds;

	Mat temp;

	thresh.copyTo(temp);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE);

	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects < util::Config::getCameraMaxNumObjects()) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat) contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area >= util::Config::getCameraMinObjectArea()) {

					Led led;

					double x, y;
					x = moment.m10 / area;
					y = moment.m01 / area;
					led.setPoint(Point(x, y));
					led.setType(theLed.getType());
					led.setColour(theLed.getColour());

					leds.push_back(led);

				}

			}
			//let user know you found an object

		}
	}

	return leds;
}

void CameraHandler::threshold(Mat source, Scalar HSVmin, Scalar HSVmax, Mat& destination) {
	inRange(source, HSVmin, HSVmax, destination);
	morphOps(destination);
}

void CameraHandler::morphOps(Mat &thresh) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);

}

void CameraHandler::captureFrame() {

	//store image to matrix
	capture->read(cameraFeed);
	//convert frame from BGR to HSV colorspace
	//darken(cameraFeed);
	cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

}

void CameraHandler::darken(Mat &image){
	for(int i=0; i < image.rows; i++) {
		for(int j=0; j < image.cols; j++) {
			// BGR
			cv::Vec3b pixel = image.at<cv::Vec3b>(i,j);
			int max = std::max(pixel[0], std::max(pixel[1], pixel[2]));
			if(max < 150) {
				pixel[0] = 0, pixel[1] = 0, pixel[2] = 0;
			} else {
				int min = std::min(pixel[0], std::min(pixel[1],pixel[2]));
				float delta = max - min;
				pixel[0] = (int) ((float) (pixel[0]-min) / delta * (float) max);
				pixel[1] = (int) ((float) (pixel[1]-min) / delta * (float) max);
				pixel[2] = (int) ((float) (pixel[2]-min) / delta * (float) max);
			}
			image.at<cv::Vec3b>(i,j) = pixel;
		}
	}
}

} /* namespace CameraHandler */
