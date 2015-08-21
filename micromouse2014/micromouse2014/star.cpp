//
//  star.cpp
//  micromouse2014
//
//

// This should be the only #include in the .cpp
#include "star.h"


#define cellsize 180.0
//#define testing 

/********** Star ********/
star::star(lidar & the_lidar, nav & the_nav):
lide(the_lidar), navigator(the_nav)
{
    deadend = false;
    
	maze.direction = "north";
	maze.compass = 90; // starting position is 90 degrees, which will be "north". 0 degrees is to the left to match up with the lidar
	maze.shift = 0.0; // default position, so shift is 0 degrees
    
	maze.startingRow = 0;
	maze.startingColumn = 0;
	atJunction = false;
	maze.xDistance = cellsize * 0.5;
	maze.yDistance = cellsize * 0.5;


	// The goal cell's attributes, assuming the goal is always the center
	maze.goalX = (maze.mazeSize / 2) * cellsize;	// halfway through the maze, multiplied by the centimeters in each cell
	maze.goalY = maze.goalX;

	rightTurns = 0;
	leftTurns = 0;

	// create the maze
//	maze.createMaze();	
}

void star::theLoop()
{
	std::map<uint, const data*>::iterator dIterate;

	cell *current;
	while (true)
	{
		lide.build_scan();
		//auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
		//auto degreeIt = (**deqIterate).begin; // degree iterator
		////(**deqIterate)[]; // how to access the individual degree
		//
		scan();
		maze.updateMaze();
		
		// move to an upcoming junction, scan & update maze some more
		decide();
		// update heuristic values
		breadthSearch();
		// decide which path to do
		depthSearch(1);

		current = maze.findCell(maze.xDistance, maze.yDistance);
		if (current->goalCell)
			break;
	}
}

bool closeEnough(double angle1, double angle2)
{
	if (abs(angle2 - angle1) <= 1.0)
		return true;
	else
		return false;
}


void polarToCartesian(double radius, double angle, double &x, double &y)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

void star::scan()
{
//#ifdef testing
    using namespace star_config;
	
	// subject to change
	double closeEnough = 1;	// in millimeters
    //Value never used, but function call is meaningful
	/*auto _360_it =*/ lide.build_scan(); // create new scan
	auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
	//auto deg_it  = (**deqIterate).begin(); // degree iterator

	map<uint, const data*>::iterator beginner;
	beginner = (**deqIterate).begin();
	while (beginner->second->invalid_data)
	{
		beginner++;
	}

	map<uint, const data*>::iterator pI;
	pI = beginner;
	
	map<uint, const data*>::iterator follower;
	follower = beginner;

	string wallOrientation;
	double wallDisplacement_x(0), wallDisplacement_y(0), distanceToWall(0);
	

	// pI is the ahead iterator. follower is right behind it. beginner is the one that 
	// is at the beginning of the current wall
    assert(follower != beginner && "Should not be the same");
	pI++;
	uint difference (-1);
	int direction = 0; // 1 = increasing distances, -1 = decreasing distances
	int initialDirection;

// prepare to go forward anyway

	// check direction for the first time, using initialDirection
	if (!(*pI).second->invalid_data && !(*follower).second->invalid_data)
		difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
	directionOfMeasurementChanges(difference, initialDirection);
    assert(difference!=uint(-1) && "Difference value was not initialized.");

	while ((*pI).second->invalid_data || (*follower).second->invalid_data)
	{
		pI++;
		follower++;
	}

	while (pI != (**deqIterate).end())
	{
		// check direction
		if (!(*pI).second->invalid_data && !(*follower).second->invalid_data)
		{
			difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
			directionOfMeasurementChanges(difference, direction);

			if ((direction != initialDirection) || (abs((*pI).second->distance - (*follower).second->distance) > closeEnough))
			{	// create a new wall if the direction of change in measurement switches, or if the difference in measurements is greater than closeEnough
#warning Value stored in "angle" is never used
				int angle = (*follower).first - (*beginner).first; //angle that encompasses the wall from the viewpoint
				if (angle < 0)
					angle = -1 * angle;

				// length from beginning spot to the last spot that was recorded as part of the same wall
				// create the wall, orient it, and add it to the maze
				wall *nWall = new wall((*beginner).second->distance, (*follower).second->distance, (*beginner).first, (*follower).first);
				maze.wallOrienter(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);
				maze.addBasedOnCompass(*nWall, wallOrientation, wallDisplacement_x, wallDisplacement_y, distanceToWall);

				pI++; // the ahead iterator
				follower++;
				beginner = follower; // set the beginner to the new wall
			}
			else
			{
				follower++;
				pI++;
			}
		}
		else
		{
			follower++;
			pI++;
		}
	}


//#endif
}


/* return true if there's a new wall. 
This is determined by iterating through the 360 degrees and 
seeing where the "derivative" changes sign. aka, increasing vs. decreasing*/
/*bool star::newWalleXists()
{
	auto _360_it = lide.build_scan(); // create new scan
	auto deqIterate = lide.scan_hist.begin(); // deque iterator for 360 scan history. points to whole scans. begin is the latest
	//auto deg_it  = (**deqIterate).begin(); // degree iterator

	// prepare to iterate through the latest scan
	map<uint, const data*>::iterator pI;
	pI = (**deqIterate).begin();
	map<uint, const data*>::iterator beginner;
	beginner = (**deqIterate).begin();
	map<uint, const data*>::iterator follower;
	follower = (**deqIterate).begin();

	// pI is the ahead iterator. follower is right behind it. beginner is the one that 
	// is at the beginning of the current wall
	pI++;

	// this deque will hold the last 5, and as soon as it notices that the values are no longer increasing/decreasing, it declares a new wall
	//std::deque<uint> checker;

	uint difference;
	int direction; // 1 = increasing distances, -1 = decreasing distances
	int initialDirection;

	// check direction for the first time, using initialDirection
	difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
	directionOfMeasurementChanges(difference, initialDirection);

	while (pI != (**deqIterate).end())
	{
		// check direction
		difference = (*pI).second->distance - (*follower).second->distance; // the difference between the follower's distance and the ahead distance 
		directionOfMeasurementChanges(difference, direction);

		if (direction != initialDirection)
		{
			return true;
		}
		
		follower++;
		pI++;
	}

	return false;
}*/

// says whether the direction of measurement differences is increasing/decreasing.
// called by scan()
void star::directionOfMeasurementChanges(uint difference, int &direction)
{   
	if (difference > 0)
	{	// the distances are increasing
		direction = 1;
	}
#if BAD_CODE//Code never used
	else if (difference < 0)
	{	// the distances are decreasing
		direction = -1;
	}
#endif
	else
	{	// the distances are the same
		direction = 0; // not likely to happen, but must include for the sake of including
	}
}







/*********** movement **********/




// decision-making at junctions. updates the map with new scans from the middle of the junction.
// does a breadth search to guess which path is faster, and gives movement costs based on this search.
// Then a breadth search from the goal to the current position is performed, updating the heuristic costs
int star::decide()
{

	uint16_t moveDistance(0), xDifference(0), yDifference(0);
#warning Value stored in "currentCell" is never used
	cell *currentCell = maze.findCell(maze.xDistance, maze.yDistance);
	/******************* Find the next upcoming junction *************************/
	char sourceDirection;
	cell *cellPoint = maze.getPointerToJunction(sourceDirection); // return sourceDirection


	// Figure out how far the robot must move to get to the center of the upcoming junction
	xDifference = abs(cellPoint->x_center - maze.xDistance);
	yDifference = abs(cellPoint->y_center - maze.yDistance);


	// choose which difference to use; x or y
	switch (sourceDirection)
	{
	case 'n':	// use the y since the robot is facing in that orientation
	case 's':
		moveDistance = yDifference;
		break;
	case 'e': // use the x since the robot is oriented that way
	case 'w':
		moveDistance = xDifference;
		break;
	default:
		break;
	}


	// An upcoming junction has been found; move to it
	if (cellPoint->declareSidesOpen(sourceDirection)) 
	{
		// latest 360 scan
		auto scanInitial = lide.scan_hist.front();
		
		// check the distance of the wall in front, before moving
		auto pack1 = scanInitial->at(lidar_config::degree_north);
		uint16_t distance1 = pack1->distance;

		/***** MOVE THIS MUCH *****/
		navigator.movedistancevariable(moveDistance);

		// update the position after checking the real change in distance

/**************************

	SLEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEP

**************************************************/

		// scan again. the difference between the scanned distances for the front 
		//	should be equal to the calculated moveDistance.
		auto scanSecond = lide.scan_hist.front();	// second scan
		auto pack2 = scanSecond->at(lidar_config::degree_north);
		uint16_t distance2 = pack2->distance;

		// get the difference between the two distances; cast them to int,
		// so that the result can be signed rather than unsigned
		int displacement_actual = int(distance1) - int(distance2);

		// updating the position accordingly
		PositionChange(displacement_actual);

		// fine tune the distance
		int correctedDistance = abs(displacement_actual - int(moveDistance));

		// go backward just a bit
		if (displacement_actual > int(moveDistance))
		{
			// move backward to make up the difference
			navigator.movedistancevariable(-correctedDistance);
			
			// update position
			PositionChange(-correctedDistance);
		}
		// go forward just a bit
		else
		{
			// move forward to make up the difference
			navigator.movedistancevariable(correctedDistance);

			// update position
			PositionChange(correctedDistance);
		}

		// declare that we are at a junction. inside the junction. Don't scan. Turn first
		atJunction = true;
	}
	else
	{
		// just go forward
		auto scannnn = lide.scan_hist.front();
		auto frontpack = scannnn->at(lidar_config::degree_north);
		moveDistance = frontpack->distance - 0.5 * nav_config::cell_size;
		navigator.movedistancevariable(moveDistance);
		PositionChange(moveDistance);
	}

	// update the maze again so that when breadth & depth searches are called, they have more information
	if (atJunction == true)
	{
		// no need to turn, just check latest scan to see and update the grid even MORE
		// READ SCAN
		scan();
		maze.updateMaze();
    }
	else
	{
		scan();
		maze.updateMaze();
	}

	return 0;
}

// called after movement, with actual measurements from lidar. 
// the value is going to positive because this is when the robot is moving forward, 
// thus the initial measurement - the second measurement is going to be positive
void star::PositionChange(double displacement_Actual)
{
	double distance = displacement_Actual;

	if ((compass > 315) && (compass < 45))
	{ // facing left
		maze.xDistance -= distance;
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction
		maze.yDistance += distance;
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing right
		maze.xDistance += distance;
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
		maze.yDistance -= distance;
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......

		// possibly a turn instruction
	}

}

void star::pushChildCellsToDeque(std::deque<cell*> &childCells)
{

}

void star::breadthSearch()
{
	int countCost = 1; // the movement cost increments every time you get to a new set of children
	cell *currentCell = maze.findCell(maze.xDistance, maze.yDistance);


	std::deque<cell*> childCells; // used for the breadth search, contains the current child cells. queue is not supported for some reason, so use in the same way. push_back, pop front



	/********** CLEAR CELL VALUES ***********/
	// actually, they'll be overwritten

	/****************************************/



	// look at child cells
	cell *cellP = currentCell;
	rose<int> sides; // north, south, east, west;
	sides = cellP->returnSides();//north, south, east, west);

	int i = 1;

	// add any cell as long as there's no wall between it and the current cell pointer
	// add their movement costs
	do{
        for(char dir : {'n','w','s','e'})
        {
            if (sides[dir] < 1)
            {
                if (!cellP->_cells[dir]->goalCell)
                {
                    cellP->_cells[dir]->movementCost = countCost;
                    cellP->_cells[dir]->sourceDirection = dir;
                    childCells.push_back(cellP->_cells[dir]);
                }
            }
        }
		// move the pointer to one of the next child cells in the 'queue'
		cellP = childCells.front();
		childCells.pop_front(); // remove the child cell from the queue

		// return the sides of the child cell excluding the source side
		sides = cellP->returnSides( cellP->sourceDirection);

		i++;
		countCost++;
	} while ((i < ((maze.mazeSize*maze.mazeSize) - 4)));	// do every cell except the goal cell


	//give the cells their new heuristic cost *************************
	// WORKING from the goal to the current position

	// look at child cells
	double xxxx = cellP->x_center, yyyy = cellP->y_center;
	cellP = maze.findClosestGoalCell(xxxx, yyyy);
    
	//why is this here?
    //int north, south, east, west;
	sides = cellP->returnSides();//north, south, east, west);

	countCost = 1;
	i = 1;

	// add any cell as long as there's no wall between it and the current cell pointer
	// add their movement costs
	do{
        
        for(char dir : {'n','w','s','e'})
        {
            if (sides[dir] < 1)
            {
                if (!cellP->_cells[dir]->goalCell)
                {
                    cellP->_cells[dir]->heuristicCost = countCost;
                    cellP->_cells[dir]->sourceDirection = dir;
                    childCells.push_back(cellP->_cells[dir]);
                }
            } 
        }
		
		// move the pointer to one of the next child cells in the 'queue'
		cellP = childCells.front();
		childCells.pop_front(); // remove the child cell from the queue

		// return the sides of the child cell excluding the source side
		sides = cellP->returnSides(cellP->sourceDirection);

		i++;
		countCost++;
	} while ((i < ((maze.mazeSize*maze.mazeSize) - 4)));	// do every cell except the current cell

}

// mode 1 = go with the "known" path
// mode 2 = go with the "unknown" path
void star::depthSearch(int mode)
{   assert(mode==1 && "Both options are the same");
	// start at current cell once again
	cell *current = maze.findCell(maze.xDistance, maze.yDistance);
	// prepare to check walls
	rose<int> sides; //int north, south, east, west;

	// check for dead end. if it's a dead end, return
	sides = current->returnSides();//north, south, east, west);
	int walls(0);// = north + south + east + west;
    for(char dir : {'n','w','s','e'}){  walls+=sides[dir];  }
	/*//if (walls >= 3)
	//{
	//	// turn around
	//	turn()
	//	return;	// dead end
	//}*/

	current->previousCell = current; // take care of the previous cell pointer

	/***** container for the possible paths *****/
	vector<path*> possiblePaths;
	path *chosenPath;
	switch (mode)
	{
        case 1:{	// go with the known path
            
            for(char dir : {'n','s','e','w'})
            {
                if (sides[dir] < 1)
                {    // a dead end will have a sum greater than the current cell
                    if (current->_cells[dir]->returnSum() <= current->returnSum()) 
                    {
                        path *p1 = new path();
                        cell *c1;
                        // move to next cell and mark its previouscell as this current one
                        c1 = current->nextCell();
                        p1->members.push_back(c1);
                        
                        while (true)
                        {	// go to the child cells and add cells with the same sum to the path
                            c1 = c1->nextCell();
                            p1->members.push_back(c1);
                            // count the unknown sides
                            p1->unknownWalls += c1->numUnknownSides(); 
                            
                            if (c1->goalCell)
                                break;
                        }
                        possiblePaths.push_back(p1);
                    }
                }
            }
            
            // choose the path with the least unknowns. just do walls in general for now
            int lowestIndex = 0;
            for (int q = 0; q < possiblePaths.size(); q++)
            {
                if (possiblePaths[q]->unknownWalls < possiblePaths[lowestIndex]->unknownWalls)
                    lowestIndex = q;
            }
            // choose the path
#warning Value stored in "chosenPath" is never used
            chosenPath = possiblePaths[lowestIndex];
            char path[4] = {'n','s','e','w'};
            assert(lowestIndex>=0 && lowestIndex<4 && "Counting error");
            turn(path[lowestIndex]);
            
        }	break;
        case 2:{	// go with the most unknown path
            for(char dir : {'n','s','e','w'})
            {
                if (sides[dir] < 1)
                {// a dead end will have a sum greater than the current cell
                    if (current->_cells[dir]->returnSum() <= current->returnSum())                     {
                        path *p1 = new path();
                        cell *c1;
                        c1 = current->nextCell();
                        p1->members.push_back(c1);
                        
                        while (true)
                        {	// go to the child cells and add cells with the same sum to the path
                            c1 = c1->nextCell();
                            p1->members.push_back(c1);
                            p1->unknownWalls += c1->numUnknownSides(); // count the unknown sides
                            
                            if (c1->goalCell)
                                break;
                        }
                        possiblePaths.push_back(p1);
                    }
                }
            }
            
            
            // choose the path with the most unknowns. just do walls in general for now
            int highestIndex = 0;
            for (int h = 0; h < possiblePaths.size(); h++)
            {
                if (possiblePaths[h]->unknownWalls > possiblePaths[highestIndex]->unknownWalls)
                    highestIndex = h;
            }
            // choose path
#warning Value stored in "chosenPath" is never used
            chosenPath = possiblePaths[highestIndex];
            char path[4] = {'n','s','e','w'};
            assert(highestIndex>=0 && highestIndex<4 && "Counting error");
            turn(path[highestIndex]);
            
        }
        default:
            break;
	}
    
}

// turn to a certain side. 0 = north. 1 = south. 2 = east. 3 = west
void star::turn(char /*int*/ direction)
{
	// assuming 90 is the starting direction. 0 is to the left at the beginning
	if ((maze.compass > 315) && (maze.compass < 45))
	{ // facing west
		switch (direction)
		{
		case 'n':	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 's':	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		case 'e':	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 'w':	// do nothing
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 45) && (maze.compass < 135))
	{ // facing north
		switch (direction)
		{
		case 'n':	// do nothing
			break;
		case 's':	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 'e':	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 'w':	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 135) && (maze.compass < 225))
	{ // facing east
		switch (direction)
		{
		case 'n':	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		case 's':	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 'e':	// do nothing
			break;
		case 'w':	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass > 225) && (maze.compass < 315))
	{	// facing south
		switch (direction)
		{
		case 'n':	// do 180 degree turn
			navigator.turnaround();
			maze.adjustCompass(180);
			break;
		case 's':	// do nothing
			break;
		case 'e':	// turn right
			navigator.turnright();
			maze.adjustCompass(90);
			break;
		case 'w':	// turn left
			navigator.turnleft();
			maze.adjustCompass(-90);
			break;
		default:
			break;
		}
	}
	else if ((maze.compass == 45) || (maze.compass == 135) || (maze.compass == 225) || (maze.compass == 315))
	{
		// hmmmm......

		// possibly a turn instruction
	}
}

/*(used in depthSearch)
return the adjacent cell that has the same sum as the current cell, meaning it's the next cell in the path.
exclude the previous cell
*/
cell *star::nextCellinPath(cell &current)
{
    cell *chosen =nullptr;
	rose<int> sides; //int north, south, east, west;
    sides = current.returnSides();//north,south,east,west);
    for(char dir : {'n','s','e','w'})
    {
        if ((sides[dir] < 1) && (current._cells[dir] != current.previousCell))
        {
            if ( (current._cells[dir]->returnSum() == current.returnSum()) ||
                 (current._cells[dir]->goalCell)                            )
                chosen = current._cells[dir];
        }
    }
    assert(chosen!=nullptr && "value should have been assigned.");
    return chosen;
}

void star::determineMovementCost(cell &ce)
{
#warning Value stored in "cellIt" is never used
	cell * cellIt;
	cellIt = maze.findCell(0, 0);

	// this isn't used, i don't think
}

// find the initial heuristic cost for all of the cells
void star::determineheuristicCost()
{
	// it's okay to measure from the exact center rather than the range of the cell in the center
	maze.goalX = maze.mazeSize * cellsize * 0.5;
	maze.goalY = maze.goalX;


}




cell * 
star::getPointerToJunction(char &sourceDirection)
{
	// get the current cell in which the robot is located
	cell * currentCell = maze.findCell(maze.xDistance, maze.yDistance);
	cell *cellPoint = currentCell;
    
	//	Check the cells in front for 
	//	where there's an upcoming junction
	//
    char face(0),rear(0);
	if ((compass > 315) && (compass < 45))
	{ // facing west
        face='w'; rear='e';
	}
	else if ((compass > 45) && (compass < 135))
	{ // default direction
        face='n'; rear='s';
	}
	else if ((compass > 135) && (compass < 225))
	{ // facing east
        face='e'; rear='w';
	}
	else if ((compass > 225) && (compass < 315))
	{ // facing south
        face='s'; rear='n';
	}
	else if ((compass == 45) || (compass == 135) || (compass == 225) || (compass == 315))
	{
		// hmmmm......
		// possibly a turn instruction
	}
    
    do{
        cellPoint = cellPoint->_cells[face];
    } while (cellPoint->declareSidesOpen(rear) == false);
    sourceDirection = rear;
    
	return cellPoint;

}


