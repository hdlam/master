/*
 * BoundingBoxHandler.cpp
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#include "BoundingBoxHandler.h"

using cv::Scalar;
using cv::Point;

BoundingBoxHandler::BoundingBoxHandler(int width, int height) {
	this->width = width;
	this->height = height;

}

BoundingBoxHandler::~BoundingBoxHandler() {

}


// bounding points

void BoundingBoxHandler::clearBoundingPoints() {
	numPoints = 0;
}

bool BoundingBoxHandler::boundingBoxComplete() {
	return numPoints == 4;
}

void BoundingBoxHandler::addBoundingPoint(Point point) {
	if(!boundingBoxComplete()) {
		boundingPoints[numPoints] = point;
		numPoints++;
	}

	if(boundingBoxComplete()) {
		updateTrapezoid();
	}

}

// convert between the real and virtual coordinate systems

void BoundingBoxHandler::updateTrapezoid() {
	Point2f resultRectangle[] = {Point2f(0,0),Point2f(1,0),Point2f(1,1),Point2f(0,1)};
	transformMatrix = cv::getPerspectiveTransform(boundingPoints, resultRectangle);
}

vector<Point2f> BoundingBoxHandler::realToVirtual(vector<Point2f> realPoints) {
	vector<Point2f> virtualPoints;
	if(realPoints.size() > 0) {
		perspectiveTransform(realPoints,virtualPoints,transformMatrix);
	}
	return virtualPoints;
}

Point2f BoundingBoxHandler::realToVirtual(Point realPoint) {
	vector<Point2f> in, out;
	in.push_back(realPoint);
	return realToVirtual(in)[0];
}

// draw bounding box

void BoundingBoxHandler::drawBoundingBox(cv::Mat &frame) {
	Scalar color(0,255,0);
	// draw trapezoid

	if(numPoints == 1) {
		cv::circle(frame, boundingPoints[0], 5, color);
	} else {
		for(int i=0; i<4 and i < numPoints - 1; i++) {
			cv::line(frame,boundingPoints[i],boundingPoints[i+1],color,4,CV_AA,0);
		}
		if(boundingBoxComplete()) {
			cv::line(frame,boundingPoints[3],boundingPoints[0],color,4,CV_AA,0);
		}
	}

}
