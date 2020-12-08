#pragma once
#include <stdio.h>
#include <cmath>
#include <stack>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Creating a shortcut for int, int pair type 
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type 
typedef pair<double, pair<int, int>> pPair;

enum MazeEntities { Wall='x', Start='A', End='B', Point='.' };

// A structure to hold the neccesary parameters 
struct cell
{
	// Row and Column index of its parent 
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
	int parent_i, parent_j;
	// f = g + h 
	double f, g, h;
	// NSEW
	char direction;
};

class AStar {
private:
	vector<vector<bool>> boolMatrix;
	Pair source;
	Pair destination;

	vector<vector<bool>> CreateBooleanMatrix(vector<vector<char>>& matrix);
	bool isValid(int row, int col);
	bool isUnBlocked(vector<vector<bool>>& matrix, int row, int col);
	bool isDestination(int row, int col, Pair dest);
	double calculateHValue(int row, int col, Pair dest);
	void tracePath(vector<vector<cell>>& cellDetails, Pair dest);

public:
	AStar(); 
	void InitializeMatrix(vector<vector<char>>& matrix);
	void PerformSearch();
	void aStarSearch(vector<vector<bool>>& matrix, Pair src, Pair dest); 
	void PrintMatrix();
};

