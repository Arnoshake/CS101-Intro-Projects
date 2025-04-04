//
//  wavefront.cpp
//

#include "wavefront.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <string.h>

void PrintDistanceMap(std::vector<std::vector<int> > map, int rows, int cols);
void PrintCharMap(std::vector<std::vector<char> > map, int rows, int cols);
bool RippleQ(std::vector<std::vector<char> > map, std::vector<std::vector<int> > &numbers, int distance, std::queue<int> &rowQ, std::queue<int> &colQ, int row, int col, char start, char goal, char boundary, char open);
bool RippleFind(std::vector<std::vector<char> > &map, std::vector<std::vector<int> > numbers, std::queue<int> &rowQ, std::queue<int> &colQ, int row, int col, int &currentDistance, bool &test);
int wavefront(std::string filename, int startrow, int startcol, int goalrow, int goalcol, std::ostream &output)
{

    char boundary = '#';
    char goal = '$';
    char start = '@';
    char open = '.';
    /*
         OPENING FILES
    */
    std::ifstream inputFile(filename);

    if (!inputFile)
    {
        std::cout << "ERROR! Could Not Open File...";
        return -1;
    }

    /*
         MAP REPRESENTATION
    */
    std::vector<std::vector<char> > map;
    std::vector<std::vector<int> > matrix;
    std::string line;
    // std::cout << "MAP VARIABLES CREATED" <<std::endl;
    while (getline(inputFile, line))
    {

        // For every line in the file, push the characters into a vector that is then pushed into map
        // simultaneously creates a second 2D vector to store visit values

        std::vector<char> row;
        std::vector<int> mathRow;

        for (int i = 0; i < line.length(); i++)
        {

            row.push_back(line.at(i));
            mathRow.push_back(-1);
        }
        map.push_back(row);
        matrix.push_back(mathRow);
    }
    inputFile.close();
    // std::cout << "MAPS CREATED" <<std::endl;
    int mapWidth = (map.at(0)).size(); // # of Col
    int mapLength = map.size();        // # of Row

    map.at(startrow).at(startcol) = start; // is it better to search from goal to start so that reversal step leads the path from beginning to end (distance calculation)
    map.at(goalrow).at(goalcol) = goal;
    matrix.at(startrow).at(startcol) = 0;

    /*
         Q DECLARATION & PROCESS
    */

    int currentRow = startrow;
    int currentCol = startcol;

    std::queue<int> rowQ;
    std::queue<int> colQ;
    int distance = 0;
    rowQ.push(currentRow);
    colQ.push(currentCol);
    distance++;

    // PrintCharMap(map,mapLength, mapWidth);
    // PrintDistanceMap(matrix,mapLength,mapWidth);

    // std::cout << "MAPS PRINTED" <<std::endl;
    while (!rowQ.empty() && !colQ.empty())
    {

        currentRow = rowQ.front();
        rowQ.pop();
        currentCol = colQ.front();
        colQ.pop();

        distance = matrix.at(currentRow).at(currentCol);

        // std::cout << "LOOP: " << count << std::endl;
        if (RippleQ(map, matrix, distance, rowQ, colQ, currentRow + 1, currentCol, start, goal, boundary, open))
        {

            // std::cout << "FOUND ABOVE" <<std::endl;
            break;
        }
        if (RippleQ(map, matrix, distance, rowQ, colQ, currentRow - 1, currentCol, start, goal, boundary, open))
        {
            // std::cout << "FOUND BELOW" <<std::endl;
            break;
        }
        if (RippleQ(map, matrix, distance, rowQ, colQ, currentRow, currentCol + 1, start, goal, boundary, open))
        {
            // std::cout << "FOUND RIGHT" <<std::endl;
            break;
        }
        if (RippleQ(map, matrix, distance, rowQ, colQ, currentRow, currentCol - 1, start, goal, boundary, open))
        {
            // std::cout << "FOUND LEFT" <<std::endl;
            break;
        }
        // std::cout << std::endl;
        // PrintDistanceMap(matrix,mapLength,mapWidth);
        // std::cout << std::endl;
    }

    std::queue<int> rowQ2;
    std::queue<int> colQ2;

    rowQ2.push(goalrow);
    colQ2.push(goalcol);

    PrintCharMap(map, mapLength, mapWidth);
    /*
    PATH MAP UPDATE
    */

    
    distance = matrix.at(goalrow).at(goalcol);
    int pathDistance = distance +1;
    int currentRow2 = goalrow;
    int currentCol2 = goalcol;
    //std::cout << distance << std::endl;

    bool test = false;
    while (!rowQ2.empty() && !colQ2.empty())
    {
        test = false;
        //std::cout << "loop" << std::endl;
        //pathDistance = matrix.at(currentRow2).at(currentCol2);
        pathDistance--;
        currentRow2 = rowQ2.front();
        currentCol2 = colQ2.front();
        rowQ2.pop();
        colQ2.pop();
        if (RippleFind(map, matrix, rowQ2, colQ2, currentRow2 + 1, currentCol2, pathDistance,test))
        {
            //std::cout << "test 1 " << std::endl;
            break;
        }
        if (test) continue;
        if (RippleFind(map, matrix, rowQ2, colQ2, currentRow2 - 1, currentCol2, pathDistance,test))
        {
            //std::cout << "test 2 " << std::endl;
            break;
        }
        if (test) continue;
        if (RippleFind(map, matrix, rowQ2, colQ2, currentRow2, currentCol2 + 1, pathDistance,test))
        {
            //std::cout << "test 3 " << std::endl;
            break;
        }
        if (test) continue;
        if (RippleFind(map, matrix, rowQ2, colQ2, currentRow2, currentCol2 - 1, pathDistance,test))
        {
            //std::cout << "test 4 " << std::endl;
            break;
        }
        if (test) continue;
        //PrintCharMap(map, mapLength, mapWidth);
    }

    
    //PrintCharMap(map, mapLength, mapWidth);
    //std::cout << std::endl;
    //PrintDistanceMap(matrix, mapLength, mapWidth);
    //std::cout << "Distance = " << distance << std::endl;
    
    for (int i = 0; i < mapLength ; i++){
        std::string tempLine;
        for (int j = 0; j <mapWidth;j++){
            tempLine = tempLine + map.at(i).at(j);
        }
        output << tempLine << std::endl;
    }

    PrintCharMap(map,mapLength,mapWidth);
    PrintDistanceMap(matrix,mapLength,mapWidth);
    return distance;
}

bool RippleQ(std::vector<std::vector<char> > map, std::vector<std::vector<int> > &numbers, int distance, std::queue<int> &rowQ, std::queue<int> &colQ, int row, int col, char start, char goal, char boundary, char open)
{
    if (map.at(row).at(col) == goal)
    {
        numbers.at(row).at(col) = distance + 1;
        // std::cout << "fail? 1" << std::endl;
        return true;
    }
    else if (map.at(row).at(col) == boundary)
    {
        // std::cout << "fail? 2" << std::endl;
        return false;
    }
    // should not be starting as the first input is that... these are checking surrounding things
    else
    {
        // std::cout << "SPOT TO ENQ..."<< std::endl;
        //  if this is not boundary or end (aka open)

        if (numbers.at(row).at(col) == -1 && map.at(row).at(col) == open)
        { // unvisited...
            rowQ.push(row);
            colQ.push(col);
            numbers.at(row).at(col) = distance + 1;

            // std::cout << "distance changes"<< std::endl;
            return false;
        }

        return false;
    }
}

void PrintDistanceMap(std::vector<std::vector<int> > map, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << (map.at(i).at(j)) % 10;
        }
        std::cout << std::endl;
    }
}

void PrintCharMap(std::vector<std::vector<char> > map, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << map.at(i).at(j);
        }
        std::cout << std::endl;
    }
}

bool RippleFind(std::vector<std::vector<char> > &map, std::vector<std::vector<int> > numbers, std::queue<int> &rowQ, std::queue<int> &colQ, int row, int col, int &currentDistance,bool &test)
{
    int wantedPath = currentDistance - 1;
    //std::cout<< "spot of current distance: " << currentDistance << " spot of interest spot " << wantedPath << std::endl;
    //map.at(row).at(col) = 'g';

    if (numbers.at(row).at(col) == wantedPath)
    {
        if (numbers.at(row).at(col) == 0)
        {
            return true;
        }
        map.at(row).at(col) = 'x';
        rowQ.push(row);
        colQ.push(col);
        test = true;
        return false;
    }
    return false;
}