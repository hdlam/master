/*
 * functions.cpp
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#include "functions.h"

namespace util {

string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}


void concatenateVectors(std::vector<Led> &v1, const std::vector<Led> &v2) {
	v1.reserve(v1.size() + distance(v2.begin(),v2.end()));
	v1.insert(v1.end(), v2.begin(), v2.end());
}

float distance(Point2f p1, Point2f p2) {
	double deltaX = p1.x - p2.x;
	double deltaY = p1.y - p2.y;
	double distance = std::sqrt(std::pow(deltaX,2) + std::pow(deltaY,2));
	return distance;
}

float rotation(Point2f source, Point2f destination) {
	float angle = std::atan2(destination.y - source.y, destination.x - source.x);
	if(angle < 0) {
		angle += M_PI * 2;
	}

	return angle;

}

Point2f unitVector(float rotation) {
	return Point2f(std::cos(rotation),std::sin(rotation));
}

}
