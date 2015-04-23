/*
 * GeneralTracker.cpp
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#include "GeneralTracker.h"
#include "../camera/BoundingBoxHandler.h"

GeneralTracker::GeneralTracker(BoundingBoxHandler *bbh) {
	this->bbh = bbh;
}

GeneralTracker::~GeneralTracker() {
	// TODO Auto-generated destructor stub
}

vector<Point2f> GeneralTracker::getVirtualPoints(vector<Led> leds) {
	vector<Point2f> ledPoints, virtualPoints;
	ledPoints.reserve(leds.size());
	for(Led led : leds) {
		ledPoints.push_back(led.getPoint());
	}

	virtualPoints = bbh->realToVirtual(ledPoints);

	for(int i = virtualPoints.size()-1; i >= 0; i--) {
		int x = virtualPoints[i].x, y = virtualPoints[i].y;
		if(x < 0 or x > 1 or y < 0 or y > 1) {
			virtualPoints.erase(virtualPoints.begin()+i);
		}
	}

	return virtualPoints;
}

double GeneralTracker::distance(Point2f p1, Point2f p2) {
	double deltaX = p1.x - p2.x;
	double deltaY = p1.y - p2.y;
	double distance = std::sqrt(std::pow(deltaX,2) + std::pow(deltaY,2));
	return distance;
}

void GeneralTracker::parseLeds(vector<Led> leds) {

	// change coordinates from real/camera space to virtual space
	vector<Point2f> currentPoints = getVirtualPoints(leds);

	// iterate over all robots in memory
	for(unsigned int r=0; r < robots.size(); r++) {
		// find the led that is closest to the robot
		double minDistance = 1E100;
		int argmin = -1;
		for(unsigned int c=0; c < currentPoints.size(); c++) {
			double d = distance(robots[r].getPosition(),currentPoints[c]);
			if(d < minDistance) {
				minDistance = d;
				argmin = c;
			}
		}

		// if the led is close enough, assume it is caused by the robot
		if(minDistance < distanceThreshold) {
			seen[r] += scoreIncreaseWhenSeen;
			if(seen[r] > scoreLimit) {
				seen[r] = scoreLimit;
			}
			history[r].push_back(currentPoints[argmin]);
			if(history[r].size() > historyLimit) {
				history[r].pop_front();
			}
			robots[r].setPosition(smooth(history[r]));
			currentPoints.erase(currentPoints.begin() + argmin);
		}
	}

	// iterate over robots and remove them if they haven't been seen in a while
	for(int r=robots.size()-1; r >= 0; r--) {
		seen[r] -= 1;

		if(seen[r] < 0) {
			history.erase(history.begin() + r);
			robots.erase(robots.begin() + r);
			seen.erase(seen.begin() + r);
		}
	}

	// assume remaining leds are new robots
	for(Point2f point : currentPoints) {
		robots.push_back(Chirp(point));
		seen.push_back(scoreIncreaseWhenSeen);
		deque<Point2f> d;
		d.push_back(point);
		history.push_back(d);
	}

}

Point2f GeneralTracker::smooth(deque<Point2f> history) {
	Point2f p1(0,0);
	for(Point2f p2 : history) {
		p1 += p2;
	}
	p1 = p1 * (1.0 / history.size());

	return p1;
}

void GeneralTracker::drawBots(cv::Mat &image) {
	Scalar red(0,0,255);
	Scalar grey(120,120,120);
	int size = image.rows;

	cv::rectangle(image,Point(0,0),Point(size,size), Scalar(100,100,100),CV_FILLED);

	vector<Chirp> verified = getRobots();
	string count = "";
	count.append(util::intToString(verified.size()));
	count.append("/");
	count.append(util::intToString(robots.size()));
	cv::putText(image,count,Point(50,50),1,2,Scalar(255,255,255));

	for(Chirp robot : robots) {
		cv::circle(image,robot.getPosition()*size,10,grey,CV_FILLED,CV_AA,0);
		cv::putText(image,util::intToString(robot.getId()),(robot.getPosition()*size) + Point2f(15,0),1,1,grey);
	}

	for(Chirp robot : verified) {
		cv::circle(image,robot.getPosition()*size,10,red,CV_FILLED,CV_AA,0);
		cv::putText(image,util::intToString(robot.getId()),(robot.getPosition()*size) + Point2f(15,0),1,1,red);
	}
}

vector<Chirp> GeneralTracker::getRobots() {
	vector<Chirp> verified;
	for(unsigned int r=0; r < robots.size(); r++) {
		if(seen[r] > scoreThreshold) {
			verified.push_back(robots[r]);
		}
	}

	return verified;
}

