//
//  cell.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
// hiiiiii

#include "cell.h"
#include <cmath>
#include "formatting.h"

using namespace std;
using namespace cell_config;

cell::cell():
_cells(nullptr,nullptr,nullptr,nullptr),
_bounds (0,0,0,0)
{
	checked = 0;
	deadend = false;
	finish = false;
}

cell::cell(int r, int c):
_cells(nullptr,nullptr,nullptr,nullptr),
_bounds (0,0,0,0),row(r),column(c)
{
	checked     = 0;
	deadend     = false;
	finish      = false;
	goalCell    = false;

	x_center    = (cellsize * column) + 0.5 * cellsize;
	y_center    = (cellsize * row) + 0.5 * cellsize;
}

// take the coordinates in centimeters and create a cell with the correct row/column attributes
cell::cell(double x, double y):
_cells(nullptr,nullptr,nullptr,nullptr),
_bounds (0,0,0,0),
row(    floor(y/cellsize)   ),
column( floor(x / cellsize) )
{
}

// uses the current direction to determine which walls are false or true
cell::cell(int left, int right, int front, string direction)
{
	deadend = false;
	finish = false;
	if (direction == "north")
	{
		_bounds.S() = true; // set to true so that when returning to the cell, it doesn't go down a path previously traveled

		if (left == 1)
			_bounds.W() = true;
		if (right == 1)
			_bounds.E() = true;
		if (front == 1)
			_bounds.N() = true;
	}
	else if (direction == "south")
	{
		_bounds.N() = true;

		if (left == 1)
			_bounds.E() = true;
		if (right == 1)
			_bounds.W() = true;
		if (front == 1)
			_bounds.S() = true;
	}
	else if (direction == "east")
	{
		_bounds.W() = true;

		if (left == 1)
			_bounds.N() = true;
		if (right == 1)
			_bounds.S() = true;
		if (front == 1)
			_bounds.E() = true;
	}
	else if (direction == "west")
	{
		_bounds.E() = true;

		if (left == 1)
			_bounds.S() = true;
		if (right == 1)
			_bounds.N() = true;
		if (front == 1)
			_bounds.W() = true;
	}
}
#ifdef UNUSED
// upon revisiting a cell, "close" the direction that you're coming from
void cell::markSourceDirection(string direction)
{
	if (direction == "north")
		_bounds.S() = true; // set to true so that when returning to the cell, it doesn't go down a path previously traveled
	else if (direction == "south")
		_bounds.N() = true;
	else if (direction == "east")
		_bounds.W() = true;
	else if (direction == "west")
		_bounds.E() = true;
}

// returns the values of left, front, right depending on the direction
void cell::checkVirtualSides(int &left, int &right, int &front, string direction)
{
	if (direction == "north")
	{
		left = _bounds.W();
		right = _bounds.E();
		front = _bounds.N();
	}
	else if (direction == "south")
	{
		left = _bounds.E();
		right = _bounds.W();
		front = _bounds.S();
	}
	else if (direction == "east")
	{
		left = _bounds.N();
		right = _bounds.S();
		front = _bounds.E();
	}
	else if (direction == "west")
	{
		left = _bounds.S();
		right = _bounds.N();
		front = _bounds.W();
	}
}
#endif
// returns true if the point is closer to sideA than sideB
bool closerTo(double sideA, double sideB, double point)
{
	double d1, d2;

	d1 = abs(sideA - point);
	d2 = abs(sideB - point);

	if (d1 < d2)
		return true;
	else
		return false;
}

// mark the wall and go to the neighboring cell and mark its corresponding wall
// mode: 1 = mark. -1 = unmark/empty
// 1 = north. 2 = south. 3 = west. 4 = east.
void cell::wallMark(char side, int mode)
{
    assert_or_throw((mode==1||mode==-1), invalid_argument, 
                    "Argument \"mode\" must be one of {+1,-1}");
    char adjacent;
    switch (side)
	{
        case 'n': adjacent='s'; break;
        case 's': adjacent='n'; break;
        case 'e': adjacent='w'; break;
        case 'w': adjacent='e'; break;
        default:    
            assert_or_throw(false, invalid_argument, 
                            "Argument \"side\" must be one of {n,e,s,w}");
	}
    
    _bounds[side] = _cells[side]->_bounds[adjacent] = ((mode==1) ? 1:0);

}

// check to see if the point is within the cell, including the walls
void cell::markWalls(double x, double y, double sourceX, double sourceY)
{
	double rightBound   = x_center + 0.5 * cellsize;
	double leftBound    = x_center - 0.5 * cellsize;
	// remember that the top left is the origin of the grid
	double topBound     = y_center + 0.5 * cellsize;
	double bottomBound  = y_center - 0.5 * cellsize;
    
    //unused
	//cell * otherCell;

	// check to see if the point is within the four walls
	if ((x >= leftBound) && (x <= rightBound))
	{
		if ((y >= bottomBound) && (y <= topBound))
		{
			if (closerTo(leftBound, rightBound, x))
			{	// closer to left wall
				if (closerTo(topBound, bottomBound, y))
				{	// closer to top wall
					if (abs(leftBound - x) < abs(topBound - y))
					{	// left wall must be present rather than the top wall
						// turn the left boundary to true, and go to that cell and change its right boundary to true
						wallMark('w', 1);
					}
					else
					{	// top wall must be present instead of the left all
						wallMark('n', 1);
					}
				}
				else
				{	// closer to bottom wall
					if (abs(leftBound - x) < abs(bottomBound - y))
					{	// left wall is present
						wallMark('w', 1);
					}
					else
					{	// bottom wall is present
						wallMark('s', 1);
					}
				}				
			}
			else
			{	// closer to right wall
				if (closerTo(topBound, bottomBound, y))
				{	// closer to top wall
					if (abs(rightBound - x) < abs(topBound - y))
					{	// right wall must be present instead of the top wall
						wallMark('e', 1);
					}
					else
					{	// top wall must be present instead of the right wall
						wallMark('n', 1);
					}
				}
				else
				{	// closer to bottom wall
					if (abs(rightBound - x) < abs(bottomBound - y))
					{	// right wall must be present instead of the bottom wall
						wallMark('e', 1);
					}
					else
					{	// bottom wall must be present instead of the right wall
						wallMark('s', 1);
					}
				}	
			}
		}
	}


	/* Naturally, when looking down an empty space, it can be assumed that for each of the cells, the wall closest to the robot is open; otherwise, it would not be possible to see through the cell
	 mark the sides closest to the robot as open only if it's unknown, to avoid error in the situations where the wall is partially seen, but not fully, blah blah. it works out in the end, with the earlier part of this function.
	 (an open wall can be closed if it's determined that a wall is there, but not the other way around) */
	declareSideEmpty(sourceX, sourceY);
}

//
void cell::declareSideEmpty(double sourceX, double sourceY)
{
	double rightBound = x_center + 0.5 * cellsize;
	double leftBound = x_center - 0.5 * cellsize;
	// remember that the top left is the origin of the grid
	double topBound = y_center + 0.5 * cellsize;
	double bottomBound = y_center - 0.5 * cellsize;




	if (closerTo(leftBound, rightBound, sourceX))
	{	// closer to left wall
		if (closerTo(topBound, bottomBound, sourceY))
		{	// closer to top wall
			if (abs(leftBound - sourceX) < abs(topBound - sourceY))
				wallMark('w', -1);
			else	
				wallMark('n', -1);
		}
		else
		{	// closer to bottom wall
			if (abs(leftBound - sourceX) < abs(bottomBound - sourceY))
				wallMark('w', -1);
			else
				wallMark('s', -1);
		}
	}
	else
	{	// closer to right wall
		if (closerTo(topBound, bottomBound, sourceY))
		{	// closer to top wall
			if (abs(rightBound - sourceX) < abs(topBound - sourceY))	
				wallMark('e', -1);
			else
				wallMark('n', -1);
		}
		else
		{	// closer to bottom wall
			if (abs(rightBound - sourceX) < abs(bottomBound - sourceY))
				wallMark('e', 1);
			else
				wallMark('s', -1);
		}
	}

}

// figure out the heuristicCost, which is simply based on the distance to the Goal.
void cell::figureheuristicCost(double goalX, double goalY)
{
	double distance;

	distance = sqrt(  ((x_center - goalX) * (x_center - goalX)) 
                    + ((y_center - goalY) * (y_center - goalY))  );

	heuristicCost = distance; // centimeters give a high number...
}

// return true if two sides other than the source side are open
bool cell::declareSidesOpen(char sourceSide)
{
	/********** IF TWO ADDITIONAL SIDES ARE OPEN, NOT JUST ONE **********/
    
    int count = 0;
	switch (sourceSide)
	{
        case 'n':
        case 's':
        case 'e':
        case 'w':
            for(auto _c: {'n','e','s','w'}){  
                if (_c==sourceSide) {   continue;   }
                if (_bounds[_c] == -1){  count++;    }
            }
            return (count>1);
        default:    
            assert_or_throw(false, invalid_argument, 
                            "Argument must be one of {n,e,s,w}");
	}
}


// return values of 1 for closed sides and -1 for open sides
rose<int>  cell::returnSides( char sourceDirection)
{
    rose<int> ret = _bounds;
    
	switch (sourceDirection)
	{
        case 'n':
        case 's':
        case 'e':
        case 'w':
            ret[sourceDirection]=1;
            return ret;
            
        default:    
            return _bounds;
	}
}

// return the number of unknown sides
int cell::numUnknownSides()
{
	int count = 0;
    for(auto _c: {'n','e','s','w'}){   
        if (_bounds[_c] == 0)
            count++;
    }
	return count;
}

// operator ==
bool cell::operator==(cell &oC)
{
	return((row == oC.row) && (column == oC.column));
}

// operator =
void 
cell::operator=(cell &c2)
{
    //copy cell structure
	row         = c2.row;
	column      = c2.column;
	x_center    = c2.x_center;
	y_center    = c2.y_center;
    _bounds     = c2._bounds;
    _cells      = c2._cells;
    //copy cell algo data
    goalCell        = c2.goalCell;
	previousCell    = c2.previousCell;
	next            = c2.next;
	movementCost    = c2.movementCost;
	heuristicCost   = c2.heuristicCost;
	sum             = c2.sum;
	sourceDirection = c2.sourceDirection;
}

// return the next cell with the same sum, if it isn't the previous cell
// also goes to a goal cell
cell * 
cell::nextCell()
{
    for(auto _c: {'n','e','s','w'})
    {   
        if(_cells[_c]->sum==sum        ?true:
           _cells[_c]->goalCell        ?true:
           _cells[_c] != previousCell  ?true: false)
        {
            _cells[_c]->previousCell = this;
            return _cells[_c];
        }
    }
    return nullptr;
}




