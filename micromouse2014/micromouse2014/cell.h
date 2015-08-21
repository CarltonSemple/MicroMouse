//
//  cell.h
//  micromouse2014
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef __micromouse2014__cell__
#define __micromouse2014__cell__

#include "wall.h"

#include "cardinal.h"

#include <cstddef>
#include <string>

class star;
class grid;


namespace cell_config 
{
    // Max cell width
    constexpr double max_width  = 180;//m
    
    // Max cell length
    constexpr double max_length = 180;//mm
    
    constexpr double cellsize = 180.0;
    constexpr double mazesize = 180.0;
    
    /// The length of the cell in inches [DEPRECIATED]
    const int CELL_SIZE =  8; 
    
    // size of cells
	//double lengthwidth;
	//double threshold;	// distance at which it's determined the side walls aren't there
}

class cell
{
    friend star;
    friend grid;
    
	int row; // lat
	int column; // long

//	cell *	north;
//	cell *	south;
//	cell *	east;
//	cell *	west;
//    
    rose<cell*>     _cells;
    
	int checked; // 1 = checked. 2 = has multiple paths...
	bool deadend;
	bool finish;
    
	double movementCost; // cost from the origin
	double heuristicCost; // estimated cost to the goal
	double sum;

	// boundaries, confirmed/uncomfirmed
	// 0 = unknown. -1 = confirmed, no wall. 1 = wall confirmed present
//	int	b_north, b_south, b_east, b_west;
    
    rose<int>       _bounds;

	bool start_node; // for cells that have multiple options for different paths
	bool state; // used to determine if the path is open or closed (already tried or not)

	double x_center, y_center;
    
    /******** goal cells *********/
	bool goalCell;
    
	char sourceDirection; // used for breadth search, so as not to go backward

public:
	cell();
	cell(int r, int c);
	cell(double x, double y);
	cell(int left, int right, int front, std::string direction);
	//cell(cell & _adj, direction _dir);

	bool operator==(cell &oC); // important
	void operator=(cell &c2);
	
	void set_adjacent(cell & _adj);
    
//	void sNorth(int g){ b_north = g;    }
//	void sSouth(int g){ b_south = g;    }
//	void sEast(int g){  b_east = g;     }
//	void sWest(int g){  b_west = g;     }
//
//	int gNorth(){   return b_north;     }
//	int gSouth(){   return b_south;     }
//	int gEast(){    return b_east;      }
//	int gWest(){    return b_west;      }
//
#ifdef UNUSED
    /// upon revisiting a cell, "close" the direction that you're coming from
	void markSourceDirection(std::string direction);
    /// returns the values of left, front, right depending on the direction
	void checkVirtualSides(int & left, 
                           int & right, 
                           int & front, 
                           std::string direction);
#endif
	void markWalls(double x, double y, double sourceX, double sourceY);
	
    /// mark the wall and go to the neighboring cell and mark its corresponding wall
    /// mode: 1 = mark. -1 = unmark/empty
    /// side: one of {n,e,s,w}
    void wallMark(char side, int mode);
	void declareSideEmpty(double sourceX, double sourceY);

	/// return true if more than the source side is open
	bool declareSidesOpen(char sourceSide);

	/// return a value of 1 for closed sides, -1 for open
	rose<int> returnSides(char sourceDirection = 0);
	int numUnknownSides();

	double returnSum() { return sum; }
    
    /// figure out the heuristicCost, which is simply based on the distance to the Goal.
	void figureheuristicCost(double goalX, double goalY);

	/// pointer to previous cell. used in depth search to make sure the process doesn't go backward
	cell * previousCell;

	/// pointer to the next cell with the same sum, used in depth search
	cell * nextCell();
	cell * next;
};


#endif /* defined(__micromouse2014__cell__) */
