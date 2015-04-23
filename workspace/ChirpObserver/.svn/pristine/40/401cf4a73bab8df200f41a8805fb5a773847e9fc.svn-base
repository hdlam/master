/*
 * Chirp.cpp
 *
 *  Created on: 28 Apr 2014
 *      Author: erik
 */

#include "Chirp.h"

#include "../util/Config.h"

using util::Config;

int Chirp::nextId = 0;

Chirp::~Chirp() {
	// TODO Auto-generated destructor stub
}

bool Chirp::overlaps(Led led) {
	float maxMoveDistance = lastSeen * Config::getRobotSpeed();
	float maxRotation = lastSeen * Config::getRobotRotation();
	float distance = util::distance(position, led.getPoint());

	switch(led.getType()) {
	case Led::RED:
		return distance + maxMoveDistance < Config::getRobotDiameter();
	case Led::BLUE:
		return distance + maxMoveDistance < Config::getRobotDiameter() / 2 + Config::getCameraLedSize();
	case Led::GREEN:
		return distance + maxMoveDistance < (1/2.0 + 1.0/4.0)  * Config::getRobotDiameter();
	}
}

bool Chirp::closeTo(Led led) {
	float maxMoveDistance = lastSeen * Config::getRobotSpeed();
	float maxRotation = lastSeen * Config::getRobotRotation();
	float distance = util::distance(position, led.getPoint());

	switch(led.getType()) {
	case Led::RED:
		return distance < maxMoveDistance;
	case Led::BLUE:
		return distance < maxMoveDistance + Config::getRobotDiameter() / 2;
	case Led::GREEN:
		// TODO: function currently assumes the robot is rotated towards the green light, use maxRotation to gain increased precision
		return distance < maxMoveDistance + Config::getRobotDiameter() / 4;
	}

}

void Chirp::assign(Led led) {
	assigned.push_back(led);
}

void Chirp::update() {
	vector<Point2f> reds, blues, greens;
	for(Led &led : assigned) {
		switch(led.getType()) {
		case Led::RED:
			reds.push_back(led.getPoint());
			break;
		case Led::BLUE:
			blues.push_back(led.getPoint());
			break;
		case Led::GREEN:
			greens.push_back(led.getPoint());
			break;
		}
	}
	assigned.clear();

	if(reds.size() > 1) {
		cerr << "Chirp::update detected too many red leds" << endl;
	}
	if(blues.size() > 8) {
		cerr << "Chirp::update detected too many blue leds" << endl;
	}
	if(greens.size() > 1) {
		cerr << "Chirp::update detected too many green leds" << endl;
	}


	if(reds.size() > 0 or blues.size() > 2 or (greens.size() > 0 and blues.size() > 1)) {
		//can pinpoint position (and possibly rotation)
		lastSeen = 1;
	} else {
		lastSeen++;
	}

	// find accurate position using red and blues
	vector<Point2f> candidatePositions;

	// all reds are candidates (supposed to be only one red!)
	candidatePositions.reserve(candidatePositions.size() + reds.size());
	candidatePositions.insert(candidatePositions.end(), reds.begin(), reds.end());


	if(blues.size() > 2) {
		for(unsigned int i=0; i < blues.size() - 2; i++) {
			for(unsigned int j=i+1; j < blues.size() -1; j++) {
				//TODO: find center of circle using blues[i], blues[j], blues[j+1], add to candidates
				/*
				Point2f middle = (blues[j] - blues[i]) * (1.0/2.0) + blues[i];
				float distance = util::distance(blues[j], blues[i]);
				float orthogonalAngle = util::rotation(blues[j], blues[i]) + M_PI / 2.0;
				if(orthogonalAngle > 2 * M_PI) {
					orthogonalAngle -= 2 * M_PI;
				}
				float orthogonalLength = std::sqrt(DIAMETER*DIAMETER / 4.0 - distance*distance);

				Point2f alternative1 = middle + util::unitVector(orthogonalAngle) * orthogonalLength;
				Point2f alternative2 = middle - util::unitVector(orthogonalAngle) * orthogonalLength;

				float distance1 = std::abs(util::distance(blues[j+1], alternative1) - DIAMETER / 2.0);
				float distance2 = std::abs(util::distance(blues[j+1], alternative2) - DIAMETER / 2.0);

				if(distance1 < distance2) {
					candidatePositions.push_back(alternative1);
				} else {
					candidatePositions.push_back(alternative2);
				}
				*/
			}
		}
	}

	if(candidatePositions.size() > 0) {
		Point2f aggregatePosition(0,0);
		for(Point2f &p : candidatePositions) {
			aggregatePosition += p;
		}

		positionHistory.push_back(aggregatePosition * (1.0 / candidatePositions.size()));
		if(positionHistory.size() > MAX_HISTORY_LENGTH) {
			positionHistory.pop_front();
		}
		position = smooth(positionHistory);
	}


	// find rotation using green relative to position

	if(greens.size() > 0) {
		rotationHistory.push_back(util::rotation(position, greens[0]));
		if(rotationHistory.size() > MAX_HISTORY_LENGTH) {
			rotationHistory.pop_front();
		}
		rotation = 0;
		for(float r : rotationHistory) {
			rotation += r;
		}
		rotation /= rotationHistory.size();
	}





	return;
}

Point2f Chirp::smooth(deque<Point2f> history) {
	Point2f p1(0,0);
	for(Point2f p2 : history) {
		p1 += p2;
	}
	p1 = p1 * (1.0 / history.size());

	return p1;
}
