#pragma once
#ifndef __WALL_H__
#define __WALL_H__

#include <array>
//#include "Room.h"

namespace Data
{
	class Room;

	class Wall
	{
	public:
		Wall();
		Wall(bool closed_value);
		~Wall();

		int Confidence();
		int Value();

		bool getClosed();
		void setClosed(bool _closed);

		// Get the parent opposite from the given one
		Room * getOtherParent(Room * parent);

		void setParent(int spouse, Room* parent);

		int known;	// starts at -1 (unknown), and increments up with more assurances

	private:
		bool closed;	// true = closed


		std::array<Room*, 2> parents;

		//friend class Room;
	};
}

#endif