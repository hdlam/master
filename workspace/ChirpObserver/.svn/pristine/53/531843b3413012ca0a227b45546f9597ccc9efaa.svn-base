/*
 * Tracker.h
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#ifndef TRACKER_H_
#define TRACKER_H_

#include <opencv/cv.h>
#include <vector>
#include <deque>
#include <cmath>
#include "../camera/Led.h"
#include "../camera/BoundingBoxHandler.h"
#include "Chirp.h"
#include "../hungarian/Hungarian.h"

namespace tracker {

class Tracker {
public:
	Tracker(BoundingBoxHandler *bbh);
	virtual ~Tracker();

	void parseLeds(vector<Led> leds);
	void drawBots(cv::Mat& image);
	void clear();

	const vector<Chirp> getRobots();

private:
	vector<Led> getVirtualPoints(vector<Led> leds);
	Point2f smooth(deque<Point2f> history);
	void categorizeLeds(vector<Led> &leds, vector<Chirp> &robots, vector<Led> &cat0, vector<Led> &cat1, vector<Led> &cat2);
	void findRobotsInCategory0(vector<Led> &cat0, vector<Led> &cat1, vector<Led> &cat2);

	BoundingBoxHandler* bbh;

	vector<Chirp> robots;
	vector<Led> leds;

	static const Scalar red;
	static const Scalar grey;
	static const Scalar orange;
	static const Scalar blue;
	static const Scalar green;
	static const Scalar black;
};

} // namespace

#endif /* TRACKER_H_ */
