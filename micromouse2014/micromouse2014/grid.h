//
//  grid.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__grid__
#define __micromouse2014__grid__


#include <iostream>
#include <vector>



//#include "wall.h"
#include "cell.h"
#include "packet.h"

namespace grid_config 
{
    /// The size of the maze in cells    
	constexpr int mazeSize = 16;
    
    constexpr int  startingRow =0, startingColumn=0;
    
    constexpr int origin_x =0 , origin_y =0;
    
    //double goalX, goalY;
}



class grid
{
	friend class star;
	
    // A pair of axis intercepts for each cell element
    // The intercept is a line of cell references
	// It should not be assumed that these are cartesian coordinates
	std::vector<cell*> lat_headers; // rows
	std::vector<cell*> long_headers; // columns
	
	cell	*	origin;
	cell	*	goal;
	cell	*	center;
    
public:
	grid();
	grid(uint x_dim, uint y_dim);
    
    /// find the current cell based on the row/column
    cell *  getCell(int _row, int _col);
    /// find the current cell based on the coordinates in centimeters
    cell *  findCell(double x, double y);
    void    addCell(cell &newcell);
#ifdef UNUSED
    void    createMaze(); // create the linked lists of cells, 16 by 16
	void    markGoalCells();
#endif
    // take the scan, add walls to the existing map of the maze, and call the 
    // decision-making function
    // the return value means
    /*********should turn to see more if the end of a wall is within the last packet*********/
    int  updateMaze();
    // get the wall's orientation relative to the robot's field of vision
    // called by updateMaze
	void wallOrienter(wall &wallInQuestion, 
                      std::string &orientation, 
                      double &x_displacement, 
                      double &y_displacement, 
                      double distanceToWall     );
    
	void addBasedOnCompass(wall &wallInQuestion, 
                           std::string wallOrientation, 
                           double &x_displacement, 
                           double &y_displacement, 
                           double distanceToWall    );
    // mark the walls of the cells along which the wall runs, and make mark of the empty walls
    // coordinateAlongWall: the coordinate along the wall is the one on which all the test points are based
    // xORy: x means x corresponds to the coordinateAlongWall. y means y corresponds to the coordinateAlongWall
    // - called by addBasedOnCompass
	void markWalls(wall &wallInQuestion, 
                   double &staticCoord, 
                   double &coordinateAlongWall, 
                   std::string &xORy        );
    
	cell *getPointerToJunction(char &sourceDirection);
	cell *findClosestGoalCell(double x, double y);
	bool closeEnough(double angle1, double angle2);

	double mazeSize;
	double cellsize;
	double compass;
	double xDistance, yDistance;
	double goalX, goalY;

	// change the compass while keeping it within 359 degrees. 
	// -values are left, + are right
	void adjustCompass(int degrees);

	// probably not needed
    std::string direction;
	double shift;
	int startingRow;
	int startingColumn;

	
};

#endif /* defined(__micromouse2014__grid__) */
