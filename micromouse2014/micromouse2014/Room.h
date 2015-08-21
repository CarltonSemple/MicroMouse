#pragma once
#ifndef __ROOM_H__
#define __ROOM_H__

#undef min
#undef max

#include <limits>
#include <vector>
#include <map>
#include "wall.h"
#include "Location.h"

namespace Data
{
	// Designates where the walls are
	enum RoomType
	{
		NONE, LEFT, BOTTOM, RIGHT, TOP,
		LEFT_BOTTOM, BOTTOM_RIGHT, LEFT_RIGHT, LEFT_TOP, BOTTOM_TOP, RIGHT_TOP,
		LEFT_BOTTOM_RIGHT, LEFT_BOTTOM_TOP, LEFT_RIGHT_TOP, BOTTOM_RIGHT_TOP, ALL
	};

	class Room
	{
	private:
		std::vector<Wall*> openings; // Vector of Wall objects
		int opens; // Number of openings
		Location loc;
		RoomType type;

		// neighboring rooms
		std::vector<Room*> neighbors;

		int breadth_heuristic;
		void reset_breadth_heuristic();
		//void set_adjacent_rooms(Room(&maze)[16][16]);

	public:
		Room() :opens(0), loc(0, 0)
		{
			for (int i = 0; i < 4; i++)
				openings.push_back(new Wall());
			breadth_heuristic = std::numeric_limits<int>::max();
			type = RoomType::ALL;
			visits = 0;
		}

		Room(int v, int x, int y);

		std::vector<bool> getOpenings();
		std::vector<Wall*>& getWalls() { return openings; }

		// Heuristics
		float weight();	// lower = better?

		int get_breadth_heuristic();
		void set_breadth_heuristic(int new_value);

		Data::Location Location() { return loc; }
		std::vector<Room*> * get_children();
		// Get the cells that are reachable and aren't the previous room
		std::vector<Room*> * get_children(Room *previous);
		// Get children that aren't already in the given collection
		std::vector<Room*> * get_children(std::vector<Room*> & existingCollection);

		/* map of booleans used in depth first search to prevent looping
		* one for each generated path*/
		std::map<int, bool> checked;

		/* Map of pointers to previous rooms for each path */
		std::map<int, Room*> previous;
		std::map<int, Room*> next;

		/***** Confidence *****/

	private:
		std::map<int, int> _alongPathConfidence;	// confidence of walls along the side of the path
		std::map<int, int> _inPathConfidence;		// confidence of walls inside path
	public:
		// Get confidence of the walls for the specified path. 
		// If edgeConfidence, return _alongPathConfidence.
		// -100: error. values weren't calculated
		int confidence(int pathNumber, bool edgeConfidence);

		/* Reset the checked, previous, next, confidence */
		void reset();
		/**********************/

		// Return 1 if there's a turn or 0 if there isn't
		int turn(int pathNumber);

		// Number of actual visits
		int visits;


		int getPassages(){ return opens; }
		char* getRoom();
		void setWall(int side, bool value);

		

		void operator =(Room & room2);
		bool operator <(Room & room2);
		bool operator >(Room & room2);
		bool operator ==(Room & room2);
		bool operator <=(Room & room2);
		bool operator >=(Room & room2);

		/*
		For two adjacent rooms on a given path, give the previous room a
		pointer to the next, and the next room a pointer to the previous.
		For the previous room, calculate the confidence for the walls along
		the path and the walls inside the path
		*/
		static void MarkPathAndCalculateWallCosts(int pathNumber, Room *previous, Room *next)
		{
			previous->next[pathNumber] = next;
			next->previous[pathNumber] = previous;

			determineInnerAndOuterWallCosts(pathNumber, previous);
		}

		float DistanceToGoal(Data::Location &goal)
		{
			float xxS = std::abs(goal.x - loc.x * std::abs(goal.x - loc.x));
			float yyS = std::abs(goal.y - loc.y * std::abs(goal.y - loc.y));
			return std::sqrt(xxS + yyS);
		}

	private:

		/* Determine a room's inner and outer wall costs (in relation to a path) using
		its already-created	map of previous-next values for a given path */
		static void determineInnerAndOuterWallCosts(int pathNumber, Room *current)
		{
			Room* nex = current->next[pathNumber];
			if (nex == nullptr)
				return;
			Room *previous = current->previous[pathNumber];
			if (previous == nullptr)
				return;

			int wallIndex1 = -1, wallIndex2 = -1;

			// Next
			if (nex->loc.y == current->loc.y)
			{
				if (nex->loc.x == current->loc.x - 1) // left
					wallIndex1 = 3;
				else if (nex->loc.x == current->loc.x + 1) // right
					wallIndex1 = 1;
			}
			else if (nex->loc.x == current->loc.x)
			{
				if (nex->loc.y == current->loc.y + 1) // top
					wallIndex1 = 2;
				else if (nex->loc.y == current->loc.y - 1) // bottom
					wallIndex1 = 0;
			}

			// Previous
			if (current->loc.y == previous->loc.y)
			{
				if (current->loc.x == previous->loc.x - 1) // left
					wallIndex2 = 3;
				else if (current->loc.x == previous->loc.x + 1) // right
					wallIndex2 = 1;
			}
			else if (current->loc.x == previous->loc.x)
			{
				if (current->loc.y == previous->loc.y + 1) // top
					wallIndex2 = 2;
				else if (current->loc.y == previous->loc.y - 1) // bottom
					wallIndex2 = 0;
			}

			calculateWallCosts(current, pathNumber, wallIndex1, wallIndex2, 'i');  // inner walls
			calculateWallCosts(current, pathNumber, wallIndex1, wallIndex2, 'o');  // outer walls
		}

		/*
		Calculate the costs of the inner walls and the walls along the sides of the path, given
		the two wall indices of the next&previous rooms in the path
		*/
		static void calculateWallCosts(Room *current, int pathNumber, int wallIndex1, int wallIndex2, char inORout)
		{
			auto w1 = current->openings[wallIndex1], w2 = current->openings[wallIndex2];
			switch (inORout)
			{
			case 'i': // inner
				current->_inPathConfidence[pathNumber] = w1->known + w2->known;
				break;
			case 'o': // outer
				for (int i = 0; i < 3; i++)
					if (i != wallIndex1 && i != wallIndex2)
						current->_alongPathConfidence[pathNumber] += current->openings[i]->known;
				break;
			default:
				return;
			}
		}
	};

}

#endif