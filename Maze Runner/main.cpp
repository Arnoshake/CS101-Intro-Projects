/*
 Name: Zachary West
 Email: zwest2563@gmail.com
 Course Section: Spring 2025 CS 201-002
 Instructions to Compile:
    I used the following statement to compile my code: "g++ -std=c++11 main.cpp wavefront.cpp -o wavefront_app"
 Instructions to Execute:
    To execute the code properly, use the following format: 
        "./wavefront_app"
    
    The results will populate within the corresponding [fileName]ByYear.txt and [fileName]ByName.txt 
        and the times for execution (used in report) will print to the terminal
*/
#include <fstream>
#include <sstream>
#include <iostream>
#include "wavefront.h"
//User added libraries...
#include <queue>
#include <string>
#include <vector>


int main(int argc, const char * argv[]) {

    std::string filename;
    std::cout<<"filename: ";
    std::cin >> filename;
    
    int srow, scol;
    std::cout << "start row: ";
    std::cin >> srow;
    std::cout<<"start col: ";
    std::cin >>scol;
    
    int xrow, xcol;
    std::cout << "goal row: ";
    std::cin >> xrow;
    std::cout<<"goal col: ";
    std::cin >> xcol;
    
    std::stringstream ss;
    int distance;
    distance = wavefront(filename,srow,scol,xrow,xcol,ss);
    std::cout << ss.str() << std::endl;
    std::cout << "Distance = " << distance << std::endl;
    
    return 0;
}
