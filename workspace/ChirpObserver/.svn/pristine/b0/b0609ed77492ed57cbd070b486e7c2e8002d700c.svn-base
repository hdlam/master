/*
 * Chirp.h
 *
 *  Created on: 28 Apr 2014
 *      Author: erik
 */

#ifndef CHIRP_H_
#define CHIRP_H_

#include <opencv/cv.h>
#include <deque>

#include "../util/functions.h"

using std::deque;
using cv::Point2f;

class Chirp {

public:

	static const int MAX_HISTORY_LENGTH = 3;

	Chirp(Point2f position=Point2f(0,0), double rotation=0) : position(position), rotation(rotation), id(nextId++){}

	virtual ~Chirp();


	int getId() const {
		return id;
	}

	const cv::Point2f getPosition() const {
		return position;
	}

	void setPosition(const cv::Point2d position) {
		this->position = position;
	}

	float getRotation() const {
		return rotation;
	}

	void setRotation(double rotation) {
		this->rotation = rotation;
	}

	bool overlaps(Led led);
	bool closeTo(Led led);
	void assign(Led led);
	void update();

	int getLastSeen() const {
		return lastSeen;
	}

private:

	Point2f smooth(deque<Point2f> history);

	Point2f position;
	float rotation;
	deque<float> rotationHistory;
	deque<Point2f> positionHistory;
	static int nextId;
	int id;
	int lastSeen = 1;
	deque<Led> assigned;
	int score = 0;
};

#endif /* CHIRP_H_ */
