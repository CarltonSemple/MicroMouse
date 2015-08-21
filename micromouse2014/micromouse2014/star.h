//
//  star.h
//  micromouse2014
//
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition

#ifndef __micromouse2014__star__
#define __micromouse2014__star__


#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <string>
#include <cmath>

#include "grid.h"
//#include "wall.h"
#include "lidar.h"
#include "nav.h"
//#include "packet.h"


namespace star_config
{
    typedef
    std::vector<cell*> path;
    
    typedef
    std::vector<packet*> packets;
    
    typedef
    std::deque<wall*> walls;
    
    typedef
    std::vector<double> vect;
    
    typedef
    std::string string;   
    
    
    constexpr int num_packets = packet_config::index_range; // 90
}


/* This will simplify data input into 19 regions. 
The angle of the data from the lidar will determine the position in viewfinder, 
	and the distance/radius will be used to determine when an open space is coming up on the left/right, or a wall ahead.
The number of items in the viewfinder may be increased for better accuracy and increased response time*/
class local_grid
{
	std::vector<double> viewfinder;
	std::vector<double> previous;
    
public:
	local_grid(): viewfinder(90), previous(90){}

	// takes the grid from the lidar scan and simplifies the data
	void updateView(grid &fetched_scan,
                    int &returnedLeft, 
                    int &returnedRight,
                    double &returnedFront       ); 
};

struct path
{

	path() { unknownWalls = 0; }
	void operator=(path &p2);

	int in_path_walls;
	int adjacent_walls;

	// making it simple for now
	int unknownWalls;

	vector<cell*> members;
};

class star
{
	grid maze;	
    
    lidar   & lide;
    nav     & navigator;
    _360_scan * vision;
    
	int rightTurns;
	int leftTurns;
    
	std::vector<cell *> traversed;

    std::vector<cell*> lat_headers; // rows
	std::vector<cell*> long_headers; // columns
    
	std::deque<wall*> wallDeq;


	double headOnDistance;
	double headOnDistance2;

	bool atJunction;
    bool deadend ;
    
    std::string direction; // direction that mouse is facing
	double compass;
    // moved to grid class: double xDistance, yDistance; // the current position of the robot
	double shift; // represents the difference between the current compass heading and the default, which is 90 degrees

    
public:
    star(lidar & the_lidar, nav & the_nav);

	// loop everything
	void theLoop();
    
    std::thread  start_thread()
    { 
        std::cout<<"\nStar started!"<<std::endl;
        return std::thread(&star::theLoop,&(*this)); 
    }

	// starting fresh
	void scan(); // modifies local grid	
		//bool newWalleXists(); // determine whether there's a new wall or not
	void directionOfMeasurementChanges(uint difference, int &direction); // called by scan();
	
		//void opposingWall();

	// algorithm, decision-making
	int decide();
	void determineMovementCost(cell &ce);
	void determineheuristicCost();
	

	// determining movementCost & heuristicCost
	void breadthSearch();
	void depthSearch(int mode);
	void turn(char /*int*/ direction);
	cell *nextCellinPath(cell &current);
	void pushChildCellsToDeque(std::deque<cell*> &childCells);
    
    
    void PositionChange(double displacement_Actual);
    cell* getPointerToJunction(char &sourceDirection);
    
};

#endif /* defined(__micromouse2014__star__) */
