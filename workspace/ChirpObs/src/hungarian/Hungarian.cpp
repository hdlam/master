/*
 * Hungarian.cpp
 *
 *  Created on: 12 May 2014
 *      Author: erik
 */

#include "Hungarian.h"

namespace hungarian {

bool visited(int vertex, deque<int> path, bool left) {

	unsigned int i = (left) ? 0 : 1;
	for(; i < path.size(); i+=2) {
		if(path[i] == vertex)
			return true;
	}

	return false;
}


deque<int> bfs(vector<vector<bool>> &adj, vector<int> &matchedWithLeft, vector<int> &matchedWithRight) {
	deque<deque<int>> queue;

	int n = adj.size();

	// populate queue with initial paths
	for(int i=0; i < n; i++) {
		if(matchedWithLeft[i] == -1) {
			deque<int> path;
			path.push_back(i);
			queue.push_back(path);
		}
	}


	while(queue.size() > 0) {

		// load path
		deque<int> path = queue.front();
		queue.pop_front();


		// find last vertex in path
		int u = path.back();
		// for all vertices adjacent to the vertex, but not yet visited
		for(int v=0; v < n; v++) {
			if(adj[u][v] and not visited(v, path, false)) {
				// create a new path to v
				deque<int> new_path(path);
				new_path.push_back(v);

				// is the right hand side vertex free?
				int matchedWith = matchedWithRight[v];
				if(matchedWith == -1) {
					// an augmenting path has been found
					return new_path;
				} else if(not visited(matchedWith, path, true)){
					// free up right hand side vertex
					new_path.push_back(matchedWith);
					queue.push_back(new_path);
				}
			}
		}
	}

	return deque<int>();
}

vector<pair<int,int>> maximumMatching(vector<vector<float>> &matrix) {

	int n = matrix.size();

	// set up adjacency matrix
	vector<vector<bool>> adj;

	for(vector<float> v : matrix) {
		adj.push_back(vector<bool>(matrix.size(), false));
	}

	for(int i=0; i < n; i++) {
		for(int j=0; j < n; j++) {
			if(matrix[i][j] < TOLERANCE) {
				adj[i][j] = true;
			}
		}
	}

	vector<int> matchedWithLeft(n,-1), matchedWithRight(n,-1);

	// TODO: perform greedy assignment for performance increase


	deque<int> augmentingPath = bfs(adj,matchedWithLeft, matchedWithRight);
	while(augmentingPath.size() > 0) {
		for(unsigned int i=1; i < augmentingPath.size(); i+=2) {
			int u = augmentingPath[i-1], v = augmentingPath[i];
			matchedWithLeft[u] = v;
			matchedWithRight[v] = u;
		}
		augmentingPath = bfs(adj,matchedWithLeft, matchedWithRight);
	}

	vector<pair<int,int>> matchings;

	for(int i=0; i < n; i++) {
		if(matchedWithLeft[i] != -1) {
			matchings.push_back(pair<int,int>(i, matchedWithLeft[i]));
		}
	}

	return matchings;
}



void markRow(vector<vector<float>> &matrix, vector<pair<int,int>> &assignment, vector<bool> &markedRows, vector<bool> &markedCols, int row) {
	if(not markedRows[row]) {
		// row is not marked
		int cols = matrix[0].size();

		markedRows[row] = true;
		for(int col=0; col < cols; col++) {
			if(not markedCols[col] and matrix[row][col] < TOLERANCE) {
				// mark column
				markedCols[col] = true;
				// mark row having assignments in column
				for(pair<int,int> p : assignment) {
					if(p.second == col) {
						markRow(matrix, assignment, markedRows, markedCols, p.first);
						break;
					}
				}
			}
		}
	}
}



vector<pair<int,int>> Hungarian(vector<vector<float>> &matrix) {

#ifndef NDEBUG

	// assert that problem dimensions are valid (n x n) and n > 0 == true

	if(matrix.size() == 0 or matrix[0].size() == 0) {
		throw std::invalid_argument("Hungarian - input matrix is empty");
	}


	unsigned int dim = matrix[0].size();

	for(vector<float> v : matrix) {
		if(v.size() != dim) {
			throw std::invalid_argument("Hungarian - requires n x n matrix, but n is not consistent for second index");
		}
	}

	if(matrix.size() != matrix[0].size()) {
		throw std::invalid_argument("Hungarian - input matrix is n x m, but algorithm requires n x n");
	}

	// requires all elements to be at least zero
	for(vector<float> v : matrix) {
		for(float f : v) {
			if(f < -TOLERANCE) {
				throw std::invalid_argument( "Hungarian - input matrix contains at least one negative number - algorithm will not work");
			}
		}
	}

#endif

	int rows = matrix.size(), cols = rows;
	unsigned int maxAssignment = rows;

	// solve trivial problems for performance gain
	if(maxAssignment == 1) {
		vector<pair<int,int>> solution;
		solution.push_back(pair<int,int>(0,0));
		return solution;
	} else if(maxAssignment == 2) {
		vector<pair<int,int>> solution;
		float cost1,cost2;
		cost1 = matrix[0][0] + matrix[1][1];
		cost2 = matrix[0][1] + matrix[1][0];
		if(cost1 <= cost2) {
			solution.push_back(pair<int,int>(0,0));
			solution.push_back(pair<int,int>(1,1));
		} else {
			solution.push_back(pair<int,int>(0,1));
			solution.push_back(pair<int,int>(1,0));
		}
		return solution;
	}

	// reduce rows
	for(vector<float> &row : matrix) {
		float min = *std::min_element(row.begin(), row.end());
		for(int col=0; col < cols; col++) {
			row[col] -= min;
		}
	}


	// reduce cols
	for(int col=0; col < cols; col++) {
		float min = std::numeric_limits<float>::max();
		for(int row=0; row < rows; row++) {
			if(matrix[row][col] < min) {
				min = matrix[row][col];
			}
		}

		for(int row=0; row < rows; row++) {
			matrix[row][col] -= min;
		}
	}


	// find assignment
	vector<pair<int,int>> assignment = maximumMatching(matrix);


	while(assignment.size() < maxAssignment) {
		vector<bool> markedRows(rows, false), markedCols(cols, false);
		vector<bool> rowIsAssigned(rows, false);

		// find assigned rows
		for(pair<int,int> p : assignment) {
			rowIsAssigned[p.first] = true;
		}

		// mark unassigned rows
		for(int row=0; row < rows; row++) {
			if(not rowIsAssigned[row]) {
				markRow(matrix, assignment, markedRows, markedCols, row);
			}
		}

		// find minimum value in marked row and unmarked column
		float minNonCovered = std::numeric_limits<float>::max();
		for(int row=0; row < rows; row++) {
			if(markedRows[row]) {
				for(int col=0; col < cols; col++) {
					if(not markedCols[col] and matrix[row][col] < minNonCovered) {
						minNonCovered = matrix[row][col];
					}
				}
			}
		}
		// reduce same elements by this value
		for(int row=0; row < rows; row++) {
			if(markedRows[row]) {
				for(int col=0; col < cols; col++) {
					if(not markedCols[col]) {
						matrix[row][col] -= minNonCovered;
					}
				}
			}
		}
		// incrase elements in (unmarked rows and marked columns)
		for(int row=0; row < rows; row++) {
			if(not markedRows[row]) {
				for(int col=0; col < cols; col++) {
					if(markedCols[col]) {
						matrix[row][col] += minNonCovered;
					}
				}
			}
		}

		// attempt a new assignment
		assignment = maximumMatching(matrix);

	}

	return assignment;
}

} // namespace hungarian

