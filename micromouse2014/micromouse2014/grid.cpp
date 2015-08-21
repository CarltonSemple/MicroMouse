//
//  grid.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "grid.h"
#include <cmath>


using namespace std;
using namespace grid_config;

grid::grid()
{
	cellsize = 180.0;
    origin = goal = center = nullptr;
    lat_headers.resize(mazeSize, nullptr);
    long_headers.resize(mazeSize, nullptr);    
	for (int r = 0; r < mazeSize; r++) // create in reverse so that the origin of the coordinates is in the bottom left
	{
		for (int c = 0; c < mazeSize; ++c)
		{
			cell *temp = new cell(r, c);
			addCell(*temp);
		}
	}
    int 
        row1 = floor(mazeSize / 2),
        row2 = row1 + 1,
        column1 = floor(mazeSize / 2),
        column2 = column1 + 1;
    
	/* mark the goal cells */
    getCell(row1, column1)->goalCell = true;
    getCell(row1, column2)->goalCell = true;
    getCell(row2, column1)->goalCell = true;
	getCell(row2, column2)->goalCell = true;
}

// find the current cell based on the row/column
cell * 
grid::getCell(int _row, int _col)
{
	cell *point;// = ( (_row< _col)?lat_headers[_row]:long_headers[_col]);  
    
    assert(_row>=0 && _row<lat_headers.size() && "Argument out of range.");
    assert(_col>=0 && _col<long_headers.size() && "Argument out of range.");
    
    assert(lat_headers[_row]!=nullptr   && "Argument invalid.");
    assert(long_headers[_col]!=nullptr  && "Argument invalid.");
    
    if (_row < _col) 
    {
        point = lat_headers[_row];
        for (int i =0; i<_col; ++i) {   point = point->_cells['e'];
        }
    }else
    {
        point = long_headers[_col];
        for (int i =0; i<_row; ++i) {   point = point->_cells['n'];
        }
    }
    
    return point ;
}
#ifdef UNUSED
// create a maze with a coordinates that start in the bottom left
void 
grid::createMaze()
{
    assert(false && "This function does nothing!");
    //moved to constructor
}

void grid::markGoalCells()
{
    assert(false && "This function does nothing!");
    //moved to constructor   
}
#endif
void grid::addCell(cell &newcell) // add in reverse...
{
	cell *adder;
	adder = lat_headers[newcell.row]; // reverse
	if (adder != nullptr)
    {   
        assert(newcell.row>0 && "Mismatch on row count.");
		for (; adder->_cells['e'] != nullptr; adder = adder->_cells['e'])
        {}
        newcell._cells['w']     = adder;
        adder->_cells['e']      = &newcell;
	}
    else
    {   assert(newcell.row==0 && "Mismatch on row init.");
		lat_headers[newcell.row] = &newcell;
    }
    
	// link it to the column
	cell *linker;
	linker = long_headers[newcell.column];
	if (linker != nullptr)
	{   
        assert(newcell.column>0 && "Mismatch on column count.");
		for(; linker->_cells['s'] != nullptr; linker = linker->_cells['s'])
        {}
        newcell._cells['n']     = linker;
        linker->_cells['s']     = &newcell;
	}
	else
    {   assert(newcell.column==0 && "Mismatch on column init.");
		long_headers[newcell.column] = &newcell;
    }
}

// find the current cell based on the coordinates in centimeters
cell *
grid::findCell(double x, double y)
{
	int row = floor(y / cellsize);
	int column = floor(x / cellsize);
    
    return getCell(row, column);
}


// get the wall's orientation relative to the robot's field of vision
// called by updateMaze
void grid::wallOrienter(wall &wallInQuestion, string &orientation, 
                        double &x_displacement, double &y_displacement, 
                        double distanceToWall                           )
{
	// only five possibilities
	double angle;
    
	if (closeEnough(wallInQuestion.leftAngle, wallInQuestion.rightAngle))
	{	// wall is directly ahead
        
		// get triangle height to see how far away it is
        
		//triangle perimeter
		double perimeter = (  wallInQuestion.radiusLeft 
                            + wallInQuestion.length 
                            + wallInQuestion.radiusRight );
#warning Value stored in "area" is never used        
		// Heron's formula for area
		double area = sqrt(  perimeter 
                           * (perimeter - wallInQuestion.radiusLeft) 
                           * (perimeter - wallInQuestion.length) 
                           * (perimeter - wallInQuestion.radiusRight)  );

		// area = (1/2) * base * height
		// height = (2*area)/base . 
        // base = wallinquestion.length. 
        // height = distanceToWall
	//UNUSED: distanceToWall = (2 * area) / wallInQuestion.length;
		x_displacement = 0;
		y_displacement = 0;
	}
	else if (wallInQuestion.leftAngle < wallInQuestion.rightAngle)
	{
		// assuming 90 degrees is directly in the middle
		if (wallInQuestion.a1 > 90.0)
		{
			// the wall is perpendicular to the robot's plane of 
            // vision (parallel with the 90 degree line)
			orientation = "perpendicularRight";
			
			angle = 180 - wallInQuestion.a2;
			x_displacement = cos(angle) * wallInQuestion.radiusRight; // tricky here
			y_displacement = sin(angle) * wallInQuestion.radiusRight;
		}
		else
		{
			// the wall is parallel to the robot's plane of 
            // vision (parallel with the 0 degree line)
			orientation = "parallelLeft";
            
			angle = wallInQuestion.a2; // rightVectorAngle.
            // x_displacement left is negative. right is positive
			x_displacement = -cos(angle) * wallInQuestion.radiusRight; 
			y_displacement = sin(angle) * wallInQuestion.radiusRight;
		}
	}
	else if (wallInQuestion.rightAngle < wallInQuestion.leftAngle)
	{
		// assuming 90 degrees is directly in the middle
		if (wallInQuestion.a2 < 90.0)
		{
			// the wall is perpendicular to the robot's plane of vision
			orientation = "perpendicularLeft";
            
			angle = wallInQuestion.a1;
            // x_displacement left is negative. right is positive
			x_displacement = -cos(angle) * wallInQuestion.radiusLeft;
			y_displacement = sin(angle) * wallInQuestion.radiusLeft;
		}
		else
		{
			// the wall is parallel to the robot's plane of vision
			orientation = "parallelRight";
            
			angle = 180 - wallInQuestion.a1;
			x_displacement = cos(angle) * wallInQuestion.radiusLeft;
			y_displacement = sin(angle) * wallInQuestion.radiusLeft;
		}
	}
    
}

// take the info of the wall from wallOrienter and add it to the map of the maze
// (calls markWalls)
void grid::addBasedOnCompass(wall &wallInQuestion, string wallOrientation, 
                             double &x_displacement, double &y_displacement, 
                             double distanceToWall                          )
{
	double x_wall, y_wall;
	string xx = "x";
	string yy = "y";
    
	// assuming 90 is the starting direction. 0 is to the left at the beginning
	if ((compass > 315) && (compass < 45))
	{ // facing left
      // these are currently the starting coordinates for the wall
      // positive y displacement will be negative x
        x_wall = xDistance - y_displacement; 	
        // positive x is to the right, which will end up being positive y
        y_wall = yDistance + x_displacement;         
        
		if ( (wallOrientation == "perpendicularRight") || 
             (wallOrientation == "perpendicularLeft")  ){
			// subtract half the wall's length from the position of the 
            // robot to get to the center of the wall
            // dividing by 2 gets the middle of the wall, which will lead 
            // to accurately marking which wall it is
			x_wall -= wallInQuestion.length / 2; 
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall -= wallInQuestion.length / 2; // left is negative y
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelRight"){
			y_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
	}
	else if ((compass > 45) && (compass < 135)) 
	{ // default direction
		x_wall = xDistance + x_displacement;
		y_wall = yDistance + y_displacement;
        
		if ( (wallOrientation == "perpendicularRight") || 
             (wallOrientation == "perpendicularLeft")  ){
			y_wall += wallInQuestion.length / 2; // now the coordinates point to the wall's midpoint
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall -= wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelRight"){
			x_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
        // positive y will equal positve x
		x_wall = xDistance + y_displacement; 
        // positive x is to the right, which will equal negative y
		y_wall = yDistance - x_displacement; 
        
		if ( (wallOrientation == "perpendicularRight") || 
             (wallOrientation == "perpendicularLeft")  ){
			x_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelLeft"){
			y_wall += wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelRight"){
			y_wall -= wallInQuestion.length / 2; // right is negative y
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		x_wall = xDistance - x_displacement; // reversed
		y_wall = yDistance - y_displacement; // reversed
        
		if ( (wallOrientation == "perpendicularRight") ||
             (wallOrientation == "perpendicularLeft")  ){
			y_wall -= wallInQuestion.length / 2;
			markWalls(wallInQuestion, x_wall, y_wall, yy);
		}
		else if (wallOrientation == "parallelLeft"){
			x_wall += wallInQuestion.length / 2; // left is positive x
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
		else if (wallOrientation == "parallelRight"){
			x_wall -= wallInQuestion.length / 2; // right is negative x
			markWalls(wallInQuestion, y_wall, x_wall, xx);
		}
	}
	else if ( (compass == 45)  || (compass == 135)  ||
              (compass == 225) || (compass == 315)  ){
		// hmmmm......
        
		// possibly a turn instruction
	}
    
	
}

// mark the walls of the cells along which the wall runs, and make mark of the empty walls
// coordinateAlongWall: the coordinate along the wall is the one on which all the test points are based
// xORy: x means x corresponds to the coordinateAlongWall. y means y corresponds to the coordinateAlongWall
// - called by addBasedOnCompass
void grid::markWalls(wall       & wallInQuestion        , 
                     double     & staticCoord           , 
                     double     & coordinateAlongWall   , 
                     string     & xORy                  ) 
{
	//************* UPDATE **********************
	// The string is actually not needed. When this function is called in 
    // "addBasedOnCompass", the x and y coordinates are switched depending 
    // on the situation
	// ******* Actually, it is needed due to how the cell->markWalls function 
    // is not written to compensate for those changes. xORy is needed
    
	// mark the wall based on the coordinates x_wall & y_wall .
	cell *cellIt;
	for (int r = 0; r < mazeSize; r++)
	{
		cellIt = lat_headers[r];
		while (cellIt != NULL)
		{
			// do multiple markings along the length of the wall
			double end = wallInQuestion.length / 2;
			double marker = coordinateAlongWall - end; // starting at one end of the wall
			while (marker <= coordinateAlongWall + end)
			{
				// marker takes the place of the coordinateAlongWall
				if (xORy == "x")
					cellIt->markWalls(marker, staticCoord, xDistance, yDistance);
				else if (xORy == "y")
					cellIt->markWalls(staticCoord, marker, xDistance, yDistance);
                
				
				marker += 0.5; // .5 centimeters should be specific enough
			}
			// move to the next cell
			cellIt = cellIt->_cells['e'];
		}
	}
    
    
}

cell * 
grid::getPointerToJunction(char & sourceDirection)
{
	// get the current cell in which the robot is located
	cell * currentCell = findCell(xDistance, yDistance);
	cell * cellPoint = currentCell;

	//	Check the cells in front for 
	//	where there's an upcoming junction
	//
    char moveTwo;
	if ((compass > 315) && (compass < 45))
	{ // facing west
        moveTwo         = 'w';
		sourceDirection = 'e';
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction	
        moveTwo         = 'n';
		sourceDirection = 's';
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing east
        moveTwo         = 'e';
		sourceDirection = 'w';
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
        moveTwo         = 's';
		sourceDirection = 'n';
	}
	else if ((compass == 45)  || (compass == 135) || 
             (compass == 225) || (compass == 315)  )
	{
		// hmmmm......
		// possibly a turn instruction
	}
    assert(moveTwo && sourceDirection && "Values must be initialized");
    do{
        cellPoint = cellPoint->_cells[moveTwo];
    } while (cellPoint->declareSidesOpen(sourceDirection) == false);
	return cellPoint;
}

cell * 
grid::findClosestGoalCell(double x, double y)
{
	int row, column;
	if ((x >= 0) && (x <= (mazeSize * cellsize * 0.5)))
	{	
		if ((y >= 0) && (y <= (mazeSize * cellsize * 0.5)))
		{
			// lower left region
			row = floor(mazeSize / 2) - 1;
			column = floor(mazeSize / 2) - 1;
			return getCell(row, column);
		}
		else
		{	// upper left region
			row = floor(mazeSize / 2);
			column = floor(mazeSize / 2) - 1;
			return getCell(row, column);
		}

	}
	else if ((x > mazeSize * cellsize * 0.5) && (x <= mazeSize))
	{	
		if ((y >= 0) && (y <= (mazeSize * cellsize * 0.5)))
		{
			// lower right region
			row = floor(mazeSize / 2) - 1;
			column = floor(mazeSize / 2);
			return getCell(row, column);
		}
		else
		{	// upper right region
			row = floor(mazeSize / 2);
			column = floor(mazeSize / 2);
			return getCell(row, column);
		}
	}


    return nullptr;
}

bool grid::closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}


// take the scan, add walls to the existing map of the maze, and call the 
// decision-making function
// the return value means
/*********should turn to see more if the end of a wall is within the last packet*********/
int 
grid::updateMaze()
{
	/**************** THE ABOVE IS IN STAR::SCAN() **************/
    
	// Add to cells the empty boundaries that must exist where it has been determined that there is no wall
	cell *cellPoint = findCell(xDistance, yDistance);
    char b_face(0), b_left(0), b_right(0);
	if ((compass > 315) && (compass < 45))
	{ // facing west
        b_left='n'; b_right='s';
        b_face='w';
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction
        b_left='e'; b_right='w';
        b_face='n';
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing east
        b_left='n'; b_right='s';
        b_face='e';
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
        b_left='e'; b_right='w';
        b_face='s';
	}
	else if ((compass == 45)  || (compass == 135) || 
             (compass == 225) || (compass == 315)  )
	{
		// hmmmm......
	}
    assert_or_throw(b_left && b_right && b_face, std::logic_error, 
                    "Compass logic failed; must assign values.");
    
    do{
        cellPoint = cellPoint->_cells[b_face];
        
        if (cellPoint->_bounds[b_left]  == 0)
            cellPoint->_bounds[b_left]  =(-1);
        if (cellPoint->_bounds[b_right] == 0)
            cellPoint->_bounds[b_right] =(-1);
        
    } while (cellPoint->_bounds[b_face] != 1);
    
	return 0;
}

// update the compass
void grid::adjustCompass(int degrees)
{
	if (degrees >= 0)
	{
		compass += degrees;
		if (compass > 359)
			compass -= 359;
	}
	else if (degrees < 0)
	{
		compass -= degrees;
		if (compass < 0){
			compass += 359;
		}
	}
}
