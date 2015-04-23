/*
 * BoundingBoxHandler.h
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#ifndef BOUNDINGBOXHANDLER_H_
#define BOUNDINGBOXHANDLER_H_

#include <opencv/cv.h>
#include "../util/functions.h"

class BoundingBoxHandler {
public:
	BoundingBoxHandler(int width, int height);
	virtual ~BoundingBoxHandler();

	void clearBoundingPoints();
	void addBoundingPoint(cv::Point point);
	bool boundingBoxComplete();

	cv::Point2f realToVirtual(cv::Point realPoint);
	vector<cv::Point2f> realToVirtual(vector<cv::Point2f> realPoints);
	void drawBoundingBox(cv::Mat &frame);
	void updateTrapezoid();

	const cv::Point2f* getBoundingPoints() const {
		return boundingPoints;
	}

private:
	int width, height;

	int numPoints = 0;
	cv::Point2f boundingPoints[4];
	Mat transformMatrix;
	int angle = 0;
};

#endif /* BOUNDINGBOXHANDLER_H_ */
