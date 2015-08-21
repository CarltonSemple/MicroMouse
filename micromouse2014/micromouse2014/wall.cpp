#include "wall.h"

namespace Data
{
	Wall::Wall()
	{
		closed = false;
		known = -1;

		// start both parents at nullptr
		for (int i = 0; i < parents.size(); i++)
			parents[i] = nullptr;
	}

	Wall::Wall(bool closed_value)
	{
		closed = closed_value;

		known = 0;	// known

		// start both parents at nullptr
		for (int i = 0; i < parents.size(); i++)
			parents[i] = nullptr;
	}



	Wall::~Wall()
	{
	}

	bool Wall::getClosed()
	{
		return closed;
	}

	void Wall::setClosed(bool _value)
	{
		closed = _value;
	}

	/**
	Get the parent opposite from the given one
	*/
	Room * Wall::getOtherParent(Room * parent)
	{
		for (auto p : parents)
		{
			if (p != parent)
				return p;
		}
		return nullptr;
	}

	// sets the designated parent (mother = 0, father = 1)
	// of the Wall object
	void Wall::setParent(int spouse, Room* parent)
	{
		parents[spouse] = parent;
	}
}