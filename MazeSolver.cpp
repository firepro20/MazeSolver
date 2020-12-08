// MazeSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "FileParser.h"
#include "AStar.h"

int main(int argc, char** argv)
{
    // Initialize objects
    FileParser fp;
    AStar aStar;
    vector<vector<char>> maze;
    // Read from the text file
    string fileName = argv[1];
    ifstream mazeFile(fileName);
    
    if (fp.CheckInputFile(mazeFile)) {
        cout << "Maze is valid!" << endl;
        fp.ConstructMatrix(mazeFile);
        fp.PrintMaze();
        maze = fp.GetMaze();
        aStar.InitializeMatrix(maze);
        aStar.PrintMatrix();
        aStar.PerformSearch();
    }
    else {
        cout << "File does not contain a valid maze or file not found!" << endl;
    }

    fp.CloseFile(mazeFile);
    system("\npause");
}