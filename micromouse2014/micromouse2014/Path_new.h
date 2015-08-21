#ifndef __PATH_NEW_H__
#define __PATH_NEW_H__

#include <deque>
#include <assert.h>
#include "ExploreMode.h"
#include "Room.h"

namespace Algorithm
{
	class Path_new
	{
	public:
		Path_new();
		Path_new(Data::Room * new_room);
		Path_new(Path_new *original_path);
		Path_new(Path_new *old_path, Data::Room * new_room);
		Path_new(Path_new *first_path, Path_new * second_path);
		~Path_new();
	private:
		static int pathCount;	// identifier for each path
	public:
		static void resetPathCount()
		{
			pathCount = 0;
		}
		int Number();

		// mark if the path is finished
		bool success;	
		bool finished;

		bool operator <(Path_new &p2);
		bool operator >(Path_new &p2);
		bool operator ==(Path_new &p2);
		bool operator <=(Path_new &p2);
		bool operator >=(Path_new &p2);
		static ExploreMode mode;
	private:
		std::deque<Data::Room*> *_rooms;
		int path_number;

		// Get the checked values in each cell corresponding to the old path
		// and copy these to the position for the new path
		void copyCheckedValues(Path_new * old_path);

		/********* Variables for calculating path weight *********/

		// Distance from beginning room to ending room
		float displacement();
	public:
		// Number of rooms
		int length();
	private:
		// Accumulated certainty from all of the cells
		float confidence(bool edges);
		int edgeCertainty;
		int innerCertainty;

		// Number of turns involved in following the path
		int turns();

	public:
		// Average distance to goal
		float averageDistanceToGoal(Data::Location &goal);

		// Getters **************************************
	public:
		std::deque<Data::Room*> * Rooms();
		std::vector<Data::Room*> * RoomsVector();
		
		/* Check if the path contains the location */
		bool Contains(int x, int y);
	};
}

#endif