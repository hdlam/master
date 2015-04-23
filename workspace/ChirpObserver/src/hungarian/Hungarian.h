/*
 * Hungarian.h
 *
 *  Created on: 12 May 2014
 *      Author: erik
 */

#ifndef HUNGARIAN_H_
#define HUNGARIAN_H_

#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <memory>
#include <iostream>
#include <stdexcept> 


using std::vector;
using std::pair;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::deque;

namespace hungarian {

const float TOLERANCE = 0.000000000001;

vector<pair<int,int>> Hungarian(vector<vector<float>> &matrix);

}

#endif /* HUNGARIAN_H_ */
