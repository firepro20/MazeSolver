#include "FileParser.h"

// Utility function to check that file exists
bool FileParser::IsFileValid(ifstream& file) {
    return file.is_open();
}

// Utility function to check if start and end exist
// 
bool FileParser::IsMazeValid(ifstream& file) {

    // Clear stream and set point to beginning of file
    file.clear();
    file.seekg(0);

    // Accepted types
    char types[] = { 'A', 'B', '.', 'x' };
    bool isStartSet = false; 
    bool isEndSet = false;

    // Create a text string, which is used to output the text file
    string readText;
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(file, readText)) {
        if (readText.find(types[0])!=string::npos)
            isStartSet = true;
        if (readText.find(types[1])!=string::npos)
            isEndSet = true;
    }
    
    return (isStartSet && isEndSet);
}

bool FileParser::CheckInputFile(ifstream& file) {
    return (IsFileValid(file) && IsMazeValid(file));
}

vector<vector<char>> FileParser::InitialiseMatrix(ifstream& file) {

    // Clear stream and set point to beginning of file
    file.clear();
    file.seekg(0);
  
    vector<vector<char>> matrix;
    char c;
    int i = 0;

    // Read file, input to matrix
    vector<char> tempVector;
    while (file.get(c)) {
        if (c != '\n') {
            tempVector.push_back(c);
        }
        else {
            matrix.push_back(tempVector);
            tempVector.clear();
        }
    }
    if (file.eof()) {
        matrix.push_back(tempVector);
        tempVector.clear();
    }
    
    return matrix;
}

void FileParser::PrintMaze() {
    if (maze.size() > 0) {
        cout << "\nSize of matrix: " << maze.size() * maze[0].size() << endl;
        for (int i = 0; i < maze.size(); i++) {
            for (int j = 0; j < maze[i].size(); j++) {
                cout << maze[i][j];
            }
            cout << endl;
        }
    }
    else {
        cout << "\nMaze is empty!" << endl;
    }
}

void FileParser::ConstructMatrix(ifstream& file) {
    maze = InitialiseMatrix(file);
}

vector<vector<char>> FileParser::GetMaze() {
    return maze;
}

void FileParser::CloseFile(ifstream& file)
{
    // Close the file
    file.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
