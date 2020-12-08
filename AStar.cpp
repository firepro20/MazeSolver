#include "AStar.h"

// Constructor
AStar::AStar()
{
    
}

void AStar::InitializeMatrix(vector<vector<char>>& matrix) {
    boolMatrix = CreateBooleanMatrix(matrix);
}

vector<vector<bool>> AStar::CreateBooleanMatrix(vector<vector<char>>& matrix) {
    vector<vector<bool>> boolMatrix(matrix.size(), vector<bool>(matrix[0].size(), 0)); // initialise with 0
    char v;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            v = matrix[i][j];
            switch (v) {
                case Wall:
                    boolMatrix[i][j] = 0;
                    break;
                case Start:
                    boolMatrix[i][j] = 1;
                    source = make_pair(i, j);
                    break;
                case End:
                    boolMatrix[i][j] = 1;
                    destination = make_pair(i, j);
                    break;
                case Point:
                    boolMatrix[i][j] = 1;
                    break;
                default:
                    break;
            }
        }
    }
    return boolMatrix;
}

// A Utility Function to check whether given cell (row, col) 
// is a valid cell or not. 
bool AStar::isValid(int row, int col)
{
    // Returns true if row number and column number 
    // is in range 
    return (row >= 0) && (row < boolMatrix.size()) &&
        (col >= 0) && (col < boolMatrix[0].size());
}

// A Utility Function to check whether the given cell is 
// blocked or not 
bool AStar::isUnBlocked(vector<vector<bool>>& matrix, int row, int col)
{
    // Returns true if the cell is not blocked else false 
    if (matrix[row][col] == 1)
        return (true);
    else
        return (false);
}

// A Utility Function to check whether destination cell has 
// been reached or not 
bool AStar::isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics. 
double AStar::calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula 
    return ((double)sqrt((row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source 
// to destination 
void AStar::tracePath(vector<vector<cell>>& cellDetails, Pair dest)
{
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;


    stack<Pair> Path;
    stack<char> dStack;
    char direction;
    char d = ' ';
    string fullPath;

    while (!(cellDetails[row][col].parent_i == row
        && cellDetails[row][col].parent_j == col))
    {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        direction = cellDetails[row][col].direction;
        dStack.push(direction);
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));

    while (!Path.empty())
    {
        pair<int, int> p = Path.top();
        if (dStack.size() > 0) {
            d = dStack.top();
            dStack.pop();
        }

        Path.pop();

        if (!(p == dest)) {
            printf("-> (%d,%d) D:%c ", p.first, p.second, d);
            fullPath += d;
        }
        else {
            printf("-> (%d,%d) ", p.first, p.second);
        }
    }
    printf("Full Path: %s\n", fullPath.c_str());
    return;
}

void AStar::PerformSearch() {
    aStarSearch(boolMatrix, source, destination);
}

// A Function to find the shortest path between 
// a given source cell to a destination cell according 
// to A* Search Algorithm 
void AStar::aStarSearch(vector<vector<bool>>& matrix, Pair src, Pair dest)
{
    // If the source is out of range 
    if (isValid(src.first, src.second) == false)
    {
        printf("Source is invalid\n");
        return;
    }

    // If the destination is out of range 
    if (isValid(dest.first, dest.second) == false)
    {
        printf("Destination is invalid\n");
        return;
    }

    // Either the source or the destination is blocked 
    if (isUnBlocked(matrix, src.first, src.second) == false ||
        isUnBlocked(matrix, dest.first, dest.second) == false)
    {
        printf("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell 
    if (isDestination(src.first, src.second, dest) == true)
    {
        printf("We are already at the destination\n");
        return;
    }

    // Create a closed list and initialise it to false which means 
    // that no cell has been included yet 
    // This closed list is implemented as a boolean 2D array 
    vector<vector<bool>> closedList(matrix.size(), vector<bool>(matrix[0].size(), 0));
    
    // Declare a 2D array of structure to hold the details 
    //of that cell 
    cell c = { -1, -1, FLT_MAX, FLT_MAX, FLT_MAX, ' ' };
    vector<vector<cell>> cellDetails(matrix.size(), vector<cell>(matrix[0].size(), c));

    int i, j;
    
    // Initialising the parameters of the starting node 
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
    cellDetails[i][j].direction = ' ';

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implenented as a set of pair of pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its 
    // 'f' as 0 
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially 
    // the destination is not reached. 
    bool foundDest = false;

    while (!openList.empty())
    {
        pPair p = *openList.begin();

        // Remove this vertex from the open list 
        openList.erase(openList.begin());

        // Add this vertex to the closed list 
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /*
         Generating all the 4 successor of this cell

                   N   
                   |   
                   |  
             W----Cell----E
                   |
                   | 
                   S   

         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West        (i, j-1)
         */

         // To store the 'g', 'h' and 'f' of the 8 successors 
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------ 

        // Only process this cell if this is a valid one 
        if (isValid(i - 1, j) == true)
        {
            // If the destination cell is the same as the 
            // current successor 
            if (isDestination(i - 1, j, dest) == true)
            {
                // Set the Parent of the destination cell 
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                cellDetails[i - 1][j].direction = 'N';
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed 
            // list or if it is blocked, then ignore it. 
            // Else do the following 
            else if (closedList[i - 1][j] == false &&
                isUnBlocked(matrix, i - 1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[i - 1][j].f == FLT_MAX ||
                    cellDetails[i - 1][j].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i - 1, j)));

                    // Update the details of this cell 
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                    cellDetails[i - 1][j].direction = 'N';
                }
            }
        }

        //----------- 2nd Successor (South) ------------ 

        // Only process this cell if this is a valid one 
        if (isValid(i + 1, j) == true)
        {
            // If the destination cell is the same as the 
            // current successor 
            if (isDestination(i + 1, j, dest) == true)
            {
                // Set the Parent of the destination cell 
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                cellDetails[i + 1][j].direction = 'S';
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed 
            // list or if it is blocked, then ignore it. 
            // Else do the following 
            else if (closedList[i + 1][j] == false &&
                isUnBlocked(matrix, i + 1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[i + 1][j].f == FLT_MAX ||
                    cellDetails[i + 1][j].f > fNew)
                {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));
                    // Update the details of this cell 
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                    cellDetails[i + 1][j].direction = 'S';
                }
            }
        }

        //----------- 3rd Successor (East) ------------ 

        // Only process this cell if this is a valid one 
        if (isValid(i, j + 1) == true)
        {
            // If the destination cell is the same as the 
            // current successor 
            if (isDestination(i, j + 1, dest) == true)
            {
                // Set the Parent of the destination cell 
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                cellDetails[i][j + 1].direction = 'E';
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed 
            // list or if it is blocked, then ignore it. 
            // Else do the following 
            else if (closedList[i][j + 1] == false &&
                isUnBlocked(matrix, i, j + 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[i][j + 1].f == FLT_MAX ||
                    cellDetails[i][j + 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i, j + 1)));

                    // Update the details of this cell 
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                    cellDetails[i][j + 1].direction = 'E';
                }
            }
        }

        //----------- 4th Successor (West) ------------ 

        // Only process this cell if this is a valid one 
        if (isValid(i, j - 1) == true)
        {
            // If the destination cell is the same as the 
            // current successor 
            if (isDestination(i, j - 1, dest) == true)
            {
                // Set the Parent of the destination cell 
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                cellDetails[i][j - 1].direction = 'W';
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed 
            // list or if it is blocked, then ignore it. 
            // Else do the following 
            else if (closedList[i][j - 1] == false &&
                isUnBlocked(matrix, i, j - 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[i][j - 1].f == FLT_MAX ||
                    cellDetails[i][j - 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i, j - 1)));

                    // Update the details of this cell 
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                    cellDetails[i][j - 1].direction = 'W';
                }
            }
        }
    }

    // When the destination cell is not found and the open 
    // list is empty, then we conclude that we failed to 
    // reach the destination cell. This may happen when the 
    // there is no way to destination cell (due to blockages) 
    if (foundDest == false)
        printf("Failed to find path to the destination cell\n");

    return;
}

void AStar::PrintMatrix()
{
    if (boolMatrix.size() > 0) {
        cout << "\nSize of matrix: " << boolMatrix.size() * boolMatrix[0].size() << endl;
        for (int i = 0; i < boolMatrix.size(); i++) {
            for (int j = 0; j < boolMatrix[i].size(); j++) {
                cout << boolMatrix[i][j];
            }
            cout << endl;
        }
    }
    else {
        cout << "Maze is empty!" << endl;
    }
}

