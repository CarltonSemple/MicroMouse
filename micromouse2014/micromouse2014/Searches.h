#ifndef __SEARCHES_H__
#define __SEARCHES_H__

//#define NOMINMAX
#undef min
#undef max

#include <queue>
#include <vector>
#include <deque>
#include <stack>
#include <map>
//#include "Cell_new.h"
#include "Filter.h"
#include "Maze.h"
#include "Path_new.h"

class Data::Maze;

namespace Algorithm
{
	/**
	* Class that contains a breadth-first search, which updates cells' heuristic values
	*/
	class Searches
	{
	public:
		// Update the cells' heuristic values starting from the given cell
		//static void breadth_first_search(Cell_new * origin);

		// Update the cells' heuristic values starting from the given cell
		static void breadth_first_search(Data::Maze & maze, int x, int y);

		/**
		* Obtain all the possible paths from a selected room
		*/
		static Path_new * depth_search(Data::Maze &maze, Path_new *actualPath, Data::Location &start, Data::Location &goal);

	private:
		// Probably will need a variable to represent when a search is in progress vs. finished
		// so that multithreading works properly.. "condition variable"


		static int depth_helper(std::map<int, std::stack<Data::Room*>> & pathStacks, std::deque<Path_new*> & paths, Path_new **bestPath, Data::Location & goal);
		static void replacePathInCollection(std::deque<Path_new*> &paths, Path_new * newPath);
	};		
}

#endif