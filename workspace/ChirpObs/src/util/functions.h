/*
 * functions.h
 *
 *  Created on: 18 Mar 2014
 *      Author: erik
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include <string>
#include <sstream>
#include <vector>
#include <opencv/cv.h>

#include "../camera/Led.h"

using std::vector;
using cv::Point2f;

namespace util {

string intToString(int number);
void concatenateVectors(vector<Led> &v1, const vector<Led> &v2);

float distance(Point2f p1, Point2f p2);
float rotation(Point2f source, Point2f destination);
Point2f unitVector(float rotation);

}
#endif /* FUNCTIONS_H_ */

