/*
 * GeneralTracker.h
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#ifndef GENERAL_TRACKER_H_
#define GENERAL_TRACKER_H_

#include <opencv/cv.h>
#include <vector>
#include <deque>
#include <cmath>
#include "../camera/Led.h"
#include "../camera/BoundingBoxHandler.h"
#include "Chirp.h"


class GeneralTracker {
public:
	GeneralTracker(BoundingBoxHandler *bbh);
	virtual ~GeneralTracker();

	void parseLeds(vector<Led> leds);
	void drawBots(cv::Mat& image);

	vector<Chirp> getRobots();

private:
	Point2f smooth(deque<Point2f> history);
	vector<Point2f> getVirtualPoints(vector<Led> leds);
	double distance(Point2f p1, Point2f p2);

	vector<Chirp> robots;
	vector<deque<Point2f>> history;
	vector<double> seen;
	BoundingBoxHandler* bbh;

	double distanceThreshold = 0.04;	// maximum distance between robot and new observation for the observation to be assigned to robot, given as percentage of the width of the camera feed
	double scoreThreshold = 50;			// score at which observed robots are counted as actual robots
	int scoreLimit = 100;				// maximum achievable score
	double scoreIncreaseWhenSeen = 5;	// score increase of robots when observed (all sources lose 1 score at every step)
	unsigned int historyLimit = 5;		// number of previous observations to use in smoothing of position

};

#endif /* GENERAL_TRACKER_H_ */
