#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <stack>
#include <set>
#include <string>
#include <vector>

using namespace std;

class FileParser {
private:

	vector<vector<char>> maze;

	bool IsFileValid(ifstream& file);
	bool IsMazeValid(ifstream& file);
	vector<vector<char>> InitialiseMatrix(ifstream& file);

public:

	bool CheckInputFile(ifstream& file);
	void PrintMaze();
	void ConstructMatrix(ifstream& file);
	vector<vector<char>> GetMaze();
	void CloseFile(ifstream& file);

};