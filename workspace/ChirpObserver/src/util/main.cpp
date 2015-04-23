/*
 * main.cpp
 *
 *  Created on: 25 Mar 2014
 *      Author: erik
 */


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <valarray>
#include <cmath>
#include <regex>

#include "functions.h"

#include "../tracker/Tracker.h"
#include "../camera/Led.h"
#include "../camera/BoundingBoxHandler.h"
#include "../camera/CameraHandler.h"
#include "../network/Communicator.h"
#include "../util/Config.h"
#include "../rs232/rs232.h"

using util::Config;

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

//names that will appear at the top of each window
const string windowNameCamera = "Original Image";
const string windowNameThreshold = "Thresholded Image";
const string windowNameVirtual = "Virtual Image";
const string windowNameTrackbar = "Trackbars";

BoundingBoxHandler *bbh;
tracker::Tracker *robotTracker;
CameraHandler::CameraHandler *ch;

void on_trackbar(int, void*) {
	//This function gets called whenever a
	// trackbar position is changed
}

void CreateTrackbars() {
	//create window for trackbars

	namedWindow(windowNameTrackbar, 0);
	//create memory to store trackbar name on window
	char trackbarName[50];
	sprintf(trackbarName, "H_MIN", H_MIN);
	sprintf(trackbarName, "H_MAX", H_MAX);
	sprintf(trackbarName, "S_MIN", S_MIN);
	sprintf(trackbarName, "S_MAX", S_MAX);
	sprintf(trackbarName, "V_MIN", V_MIN);
	sprintf(trackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar("H_MIN", windowNameTrackbar, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", windowNameTrackbar, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", windowNameTrackbar, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", windowNameTrackbar, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", windowNameTrackbar, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", windowNameTrackbar, &V_MAX, V_MAX, on_trackbar);

}

void MouseCallbackFunc(int event, int x, int y, int flags, void* userdata) {

	using namespace cv;
	switch (event) {
	case EVENT_LBUTTONUP:
		if (not bbh->boundingBoxComplete()) {
			bbh->addBoundingPoint(Point(x, y));
		}
		break;
	case EVENT_MBUTTONUP:
		break;
	case EVENT_RBUTTONDBLCLK:
		bbh->clearBoundingPoints();
		robotTracker->clear();
		break;
	}
}

void drawLeds(vector<Led> theLeds, Mat &frame) {

	for(Led led : theLeds) {

		Point point = led.getPoint();

		// circle
		cv::circle(frame, cv::Point(point.x, point.y), 5, led.getColour());

		// color
		string color;
		switch(led.getType()) {
		case Led::RED: color = "red"; break;
		case Led::GREEN: color = "green"; break;
		case Led::BLUE: color = "blue"; break;
		case Led::CALIBRATION: color = "calibration"; break;
		}

		cv::putText(frame, color, cv::Point(point.x+10, point.y - 20), 1, 1, led.getColour());
	}
}

// temporary network code
void messageHandler(std::vector<Chirp> robots) {
	std::cout << "Received: " << endl;
	for(const Chirp &i : robots) {
		std::cout << "\t#" << i.getId() << "\t(" << i.getPosition().x << ',' << i.getPosition().y << ')' << endl;
	}
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::string item;

    // use stdlib to tokenize the string
    std::stringstream ss(s);
    while (std::getline(ss, item, delim))
        if(!item.empty())
            elems.push_back(item);

    return elems;
}

void extractInformation(std::string received, int &id, double &x, double &y, double &r)
{
	std::vector<std::string> robotsInfo = split(received,';');
	for (std::string robotInfo : robotsInfo)
	{
		std::vector<std::string> fields = split(robotInfo,':');
		std::regex xReg("X.*",std::regex_constants::basic);
		std::regex yReg("Y.*",std::regex_constants::basic);
		std::regex rReg("R.*",std::regex_constants::basic);
		std::regex doubleReg(".*\\..*",std::regex_constants::basic);
		bool x_found = false;
		bool y_found = false;
		bool r_found = false;
		for (std::string field : fields)
		{
			std::smatch x_match;
			std::smatch y_match;
			std::smatch r_match;
			std::smatch double_match;
			if (std::regex_match(field, x_match, xReg))
			{
						field.erase(0,1);
						id = atoi(field.c_str());
						x_found = true;
			}
			if (std::regex_match(field, y_match, yReg))
			{
						field.erase(0,1);
						int y_id = atoi(field.c_str());
						//if the id of the robot in y doesn't match the id of the robot in x, something went wrong with the tram
						//therefore abort
						if (y_id != id)
						{
							return;
						}
						y_found = true;
			}
			if (std::regex_match(field, r_match, rReg))
			{
						field.erase(0,1);
						int r_id = atoi(field.c_str());
						//if the id of the robot in y doesn't match the id of the robot in x, something went wrong with the tram
						//therefore abort
						if (r_id != id)
						{
							return;
						}
						r_found = true;
			}
			if (std::regex_match(field, double_match, doubleReg))
			{
				if(x_found == true)
				{
					x = atof(field.c_str());
				}
				if(y_found == true)
				{
					y = atof(field.c_str());
				}
				if(r_found == true)
				{
					r = atof(field.c_str());
				}
			}
		}
	}
}

void messageHandlerAscii(std::string received) 
{
	std::cout << "Received: " << received <<  std::endl;
	std::vector<std::string> robotsInfo = split(received,'|');
	for (std::string robotInfo : robotsInfo)
	{
		int id = -1;
		double x = 0.0;
		double y = 0.0;
		double r = 0.0;
		extractInformation(robotInfo,id,x,y,r);
		std::cout << id << " " << x << " " << y << " " << r << std::endl;
	}

	//com serial
	/*
	std::string order ("fwrd ");
  unsigned char * cstr = new unsigned char [order.length()+1];
  memcpy(cstr, order.c_str(), order.size());
	cstr[order.size()]=0;
	RS232_SendBuf(0,cstr,5);
	delete cstr;
	*/
}

int networkMain(int argc, char* argv[]) {
	cout << "starting receiver" << endl;
	network::Communicator<Chirp> communicator;
	communicator.listenAscii(messageHandlerAscii,52346);
	//com serial
	//RS232_CloseComport(Config::getComPortId());
	cout << "finished" << endl;
	return 0;
}

bool askQuestion(string question, bool yesIsDefault) {
	cout << question << ' ';
	if(yesIsDefault) {
		cout << "(Y/n) ";
	} else {
		cout << "(y/N) ";
	}

	string answer;
	getline(cin, answer);
	if(answer.length() == 0)
		return yesIsDefault;
	else if(yesIsDefault)
		return answer.at(0) != 'n';
	else
		return answer.at(0) == 'y';

}

// end temporary network code

int main(int argc, char* argv[]) {

	srand(time(0));

	if(argc > 1) {
		cout << "Loading configuration from \"" << argv[1] << "\"" << endl;
		if(not Config::loadConfig(argv[1])) {
			return 1;
		}
	} else {
		cerr << "Configuration file not provided" << endl;
		return 2;
	}


	// temporary network code
	if(Config::isNetworkEnabled() and
			askQuestion("Start receiver instead of sender (client instead of server)?", false)) {
		//com serial
		//RS232_OpenComport(Config::getComPortId(),9600);
		return networkMain(argc, argv);
	}


	network::Communicator<Chirp> communicator;
	if(Config::isNetworkEnabled()) {
		communicator.connect(Config::getNetworkAddress(), util::intToString(Config::getNetworkPort()));
	}

	if (Config::isCalibrationEnabled()) {
		//create slider bars for HSV filtering
		CreateTrackbars();
	}


	bbh = new BoundingBoxHandler(Config::getCameraWidth(), Config::getCameraHeight());
	ch = new CameraHandler::CameraHandler(Config::getCameraWidth(), Config::getCameraHeight());
	robotTracker = new tracker::Tracker(bbh);

	ch->initializeCapture(Config::getCameraDeviceId());

	// setup mouse listener
	namedWindow(windowNameCamera);
	cv::setMouseCallback(windowNameCamera, MouseCallbackFunc, NULL);

	// setup colors
	Led green(Led::GREEN), red(Led::RED), blue(Led::BLUE), calibration(Led::CALIBRATION);
	red.setHSVmin(Config::getRedHsVmin());
	red.setHSVmax(Config::getRedHsVmax());

	green.setHSVmin(Config::getGreenHsVmin());
	green.setHSVmax(Config::getGreenHsVmax());

	blue.setHSVmin(Config::getBlueHsVmin());
	blue.setHSVmax(Config::getBlueHsVmax());

	
	Mat cameraFeed;
	Mat virtualFeed(Config::getVirtualFeedWidth(), Config::getVirtualFeedHeight(), CV_8UC3, Scalar(0, 0, 0));
	
	vector<Led> leds;
	while (true) {
		leds.clear();

		ch->captureFrame();
		cameraFeed = ch->getCameraFeed();

		// find leds
		if(Config::isCalibrationEnabled()) {
			calibration.setHSVmin(Scalar(H_MIN,S_MIN,V_MIN));
			calibration.setHSVmax(Scalar(H_MAX,S_MAX,V_MAX));
			util::concatenateVectors(leds,ch->findObjects(calibration));
		} else {
			util::concatenateVectors(leds,ch->findObjects(red));
			util::concatenateVectors(leds,ch->findObjects(green));
			util::concatenateVectors(leds,ch->findObjects(blue));
		}

		drawLeds(leds, cameraFeed);
		bbh->drawBoundingBox(cameraFeed);

		if(bbh->boundingBoxComplete()) {
			robotTracker->parseLeds(leds);
			if(Config::isNetworkEnabled()) {
				std::vector<Chirp> bots = robotTracker->getRobots();
				//communicator.send(bots);
				communicator.sendAscii(bots);
			}

		}
		robotTracker->drawBots(virtualFeed);

		imshow(windowNameCamera, cameraFeed);
		imshow(windowNameVirtual, virtualFeed);

		if(Config::isCalibrationEnabled()) {
			imshow(windowNameThreshold, ch->getThresholdFeed());
		}

		//delay n milliseconds so that screen can refresh.
		//image will not appear without this waitKey() command
		char key = waitKey(Config::getFrameDelay());

		if(key == '-1') {
			// no key - take no action
		} else if(key == '') {
			// escape - quit
			return 0;
		} else if(key == Config::getControlLoadConfig()) {
			Config::loadConfig(argv[1]);
		} else {
			// unknown key pressed - take no action
		}
	}

	return 0;
}
