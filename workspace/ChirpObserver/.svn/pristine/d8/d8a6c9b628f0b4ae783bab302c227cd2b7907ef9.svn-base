/*
 * Tracker.cpp
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#include "Tracker.h"
#include "../util/functions.h"
#include "../util/Config.h"
#include <cmath>

namespace tracker {

const Scalar Tracker::red = Scalar(0,0,255);
const Scalar Tracker::grey = Scalar(120,120,120);
const Scalar Tracker::orange = Scalar(0,165,255);
const Scalar Tracker::blue = Scalar(255,0,0);
const Scalar Tracker::green = Scalar(0,255,0);
const Scalar Tracker::black = Scalar(0,0,0);

Tracker::Tracker(BoundingBoxHandler *bbh) : bbh(bbh) {
	robots.reserve(util::Config::getNumRobots());
}

Tracker::~Tracker() {
}

vector<Led> Tracker::getVirtualPoints(vector<Led> leds) {
	vector<Point2f> ledPoints;
	vector<Point2f> virtualPoints;
	vector<Led> virtualLeds;
	ledPoints.reserve(leds.size());
	for(Led led : leds) {
		ledPoints.push_back(led.getPoint());
	}

	virtualPoints = bbh->realToVirtual(ledPoints);

	for(int i = virtualPoints.size()-1; i >= 0; i--) {
		float x = virtualPoints[i].x, y = virtualPoints[i].y;
		if(x > 0 and x < 1 and y > 0 and y < 1) {
			Led l(leds[i]);
			l.setPoint(virtualPoints[i]);
			virtualLeds.push_back(l);
		}
	}

	return virtualLeds;
}

void Tracker::categorizeLeds(vector<Led> &leds, vector<Chirp> &robots, vector<Led> &cat0, vector<Led> &cat1, vector<Led> &cat2) {
	bool allPlaced = (robots.size() == util::Config::getNumRobots());
	for(Led &led : leds) {
		int closeTo = 0;
		bool overlap = false;
		int assignedToRobot = -1;

		for(int i=0; i < robots.size(); i++) {
			if(robots[i].overlaps(led)) {
				overlap = true;
				if(robots[i].closeTo(led)) {
					assignedToRobot = i;
					closeTo = 1;
				}
				break;
			}
			if(robots[i].closeTo(led)) {
				closeTo++;
				assignedToRobot = i;
			}
		}

		switch(closeTo) {
		case 0:
			cat0.push_back(led);
			break;
		case 1:
			if(allPlaced) {
				if(assignedToRobot != -1) {
					robots[assignedToRobot].assign(led);
				} else {
					// must be a false positive
				}
			} else if(not overlap) {
				cat1.push_back(led);
			} else {
				if(assignedToRobot != -1) {
					robots[assignedToRobot].assign(led);
				}
			}
			break;
		default:
			cat2.push_back(led);
		}
	}
}

void Tracker::findRobotsInCategory0(vector<Led> &cat0, vector<Led> &cat1, vector<Led> &cat2) {
	if(robots.size() == util::Config::getNumRobots()) {
		return;
	}

	vector<Chirp> newRobots;
	for(Led &l : cat0) {
		if(l.getType() == Led::RED) {
			Chirp newRobot;
			newRobot.setPosition(l.getPoint());
			newRobots.push_back(newRobot);
		}
	}
	vector<Led> newCat0;
	categorizeLeds(cat0, newRobots, newCat0, cat1, cat2);
	cat0 = newCat0;

	robots.reserve(robots.size() + newRobots.size());
	robots.insert(robots.end(), newRobots.begin(), newRobots.end());

	// TODO: also add robots using only blues and greens

}

void Tracker::parseLeds(vector<Led> detectedLeds) {

	// change coordinates from real/camera space to virtual space
	leds = getVirtualPoints(detectedLeds);
	vector<Led> cat0, cat1, cat2;
	categorizeLeds(leds, robots, cat0, cat1, cat2);

	// handle category 0
	findRobotsInCategory0(cat0,cat1,cat2);

	// handle category 1
	if(robots.size() == util::Config::getNumRobots()) {
		// assign all leds in category 1
		categorizeLeds(cat1, robots, cat0, cat1, cat2);
		cat1.clear();
	}

	for(Chirp &robot : robots) {
		robot.update();
	}


	// evict robots
	for(int i=robots.size()-1; i >= 0; i--) {
		if(robots[i].getLastSeen() > util::Config::getEvictionLimit()) {
			robots.erase(robots.begin()+i);
		}
	}
}

void Tracker::drawBots(cv::Mat &image) {
	int size = image.rows;

	int robotSize = (int) ((float) size * util::Config::getRobotDiameter() / 2.0);
	int ledSize = (int) ((float) size * (float) util::Config::getCameraLedSize() / 2.0);

	if (robotSize == 0) {
		robotSize++;
	}
	if (ledSize == 0) {
		ledSize++;
	}


	if(! bbh->boundingBoxComplete()) {
		cv::rectangle(image,Point(0,0),Point(size,size), Scalar(0,0,0),CV_FILLED);
		return;
	}

	// grey background
	cv::rectangle(image,Point(0,0),Point(size,size), Scalar(100,100,100),CV_FILLED);

	// robots

	for(Chirp robot : robots) {
		cv::circle(image, robot.getPosition()*size,robotSize,black,CV_FILLED,CV_AA,0);
		Point2f center = robot.getPosition() * size;
		Point2f end = center + util::unitVector(robot.getRotation()) * robotSize;
		cv::line(image, center, end, orange, ledSize);
		cv::putText(image,util::intToString(robot.getId()),robot.getPosition()*size+Point2f(2*robotSize,0), 1,1,orange);
	}

	// draw red and green observations
	for(Led l : leds){

		Scalar color;
		switch(l.getType()) {
		case Led::RED: color = red; break;
		case Led::BLUE: color = blue; break;
		case Led::GREEN: color = green; break;
		}

		cv::circle(image, l.getPoint() * size, ledSize, color, CV_FILLED, CV_AA, 0);
	}

	cv::putText(image,util::intToString(robots.size()),Point(50,50),1,2,black);
}

void Tracker::clear() {
	robots.clear();
	leds.clear();
}

const vector<Chirp> Tracker::getRobots() {
	return robots;
}

} //namespace
