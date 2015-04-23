/*
 * CameraHandler.h
 *
 *  Created on: 25 Mar 2014
 *      Author: erik
 */

#ifndef CAMERAHANDLER_H_
#define CAMERAHANDLER_H_

namespace CameraHandler {


class CameraHandler {
public:

	CameraHandler(int frameWidth, int frameHeight);
	virtual ~CameraHandler();
	bool initializeCapture(int device);
	void captureFrame();
	void threshold(Mat source, Scalar HSVmin, Scalar HSVmax, Mat& destination);
	void darken(Mat &image);
	vector<Led> findObjects(Led theLed);
	void morphOps(Mat &thresh);

	const Mat& getCameraFeed() const {
		return cameraFeed;
	}

	const Mat& getThresholdFeed() const {
		return thresh;
	}

private:

	VideoCapture* capture = nullptr;
	int frameWidth;
	int frameHeight;
	int maxObjectArea;


	//Matrices to store each frame of the webcam feed
	Mat cameraFeed;
	Mat HSV;
	Mat thresh;
};

} /* namespace CameraHandler */

#endif /* CAMERAHANDLER_H_ */
