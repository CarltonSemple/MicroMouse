#include "Room.h"

namespace Data
{
	Room::Room(int v, int x, int y) :opens(0), loc(x, y)
	{
		// Variable v determine which kind of room will be made.
		// Why did I name it v? I have no idea..

		for (int i = 0; i < 4; i++)
			openings.push_back(new Wall());
		type = (v > 0) ? (RoomType)(v - 1) : (RoomType)(15);
		switch (v)
		{
		case 1: // none
			opens = 4; break;
		case 2: // left
			openings[0]->setClosed(1); opens = 3; break;
		case 3: // bottom
			openings[1]->setClosed(1); opens = 3; break;
		case 4: // right
			openings[2]->setClosed(1); opens = 3; break;
		case 5: // top
			openings[3]->setClosed(1); opens = 3; break;
		case 6: // left bottom
			openings[0]->setClosed(1); openings[1]->setClosed(1); opens = 2; break;
		case 7: // bottom right
			openings[1]->setClosed(1); openings[2]->setClosed(1); opens = 2; break;
		case 8: // left right
			openings[0]->setClosed(1); openings[2]->setClosed(1); opens = 2; break;
		case 9: // left top
			openings[0]->setClosed(1); openings[3]->setClosed(1); opens = 2; break;
		case 10: // bottom top
			openings[1]->setClosed(1); openings[3]->setClosed(1); opens = 2; break;
		case 11: // right top
			openings[2]->setClosed(1); openings[3]->setClosed(1); opens = 2; break;
		case 12: // left bottom right
			openings[0]->setClosed(1); openings[1]->setClosed(1); openings[2]->setClosed(1); opens = 1; break;
		case 13: // left bottom top
			openings[0]->setClosed(1); openings[1]->setClosed(1); openings[3]->setClosed(1); opens = 1; break;
		case 14: // left right top
			openings[0]->setClosed(1); openings[2]->setClosed(1); openings[3]->setClosed(1); opens = 1; break;
		case 15: // bottom right top
			openings[1]->setClosed(1); openings[2]->setClosed(1); openings[3]->setClosed(1); opens = 1; break;

		default:
			for (int i = 0; i < 4; i++) { openings[i]->setClosed(1); } break;
		}

		for (int i = 0; i < 4; i++)
			openings[i]->setParent(0, this);

		visits = 0;
	}

	char* Room::getRoom()
	{
		// char topLeftCorner = 201;
		// char topRightCorner = 187;
		// char bottomLeftCorner = 200;
		// char bottomRightCorner = 188;
		// char hoLine = 205;
		// char vertLine = 186;

		char* room = new char(9);
		switch (type + 1)
		{
		case 1: // none
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			break;
		case 2: // left
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			break;
		case 3: // bottom
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			break;
		case 4: // right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			break;
		case 5: // top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			break;
		case 6: // left bottom
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			break;
		case 7: // bottom right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			break;
		case 8: // left right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			break;
		case 9: // left top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			break;
		case 10: // bottom top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			break;
		case 11: // right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			break;
		case 12: // left bottom right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 200; room[7] = 205; room[8] = 188;
			break;
		case 13: // left bottom top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			break;
		case 14: // left right top
			room[0] = 201; room[1] = 205; room[2] = 187; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			break;
		case 15: // bottom right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			break;
		default:
			for (int i = 0; i < 9; i++) { room[i] = 176; } break;
		}
		return room;
	}

	void Room::setWall(int side, bool value)
	{
		if (value == openings[side]->getClosed())
			return;
		openings[side]->setClosed(value);
		if (value)
		{
			opens--;
			if (side == 0) // add wall to left
			{
				if (type == RoomType::NONE)
					type = RoomType::LEFT;
				else if (type == RoomType::BOTTOM)
					type = RoomType::LEFT_BOTTOM;
				else if (type == RoomType::RIGHT)
					type = RoomType::LEFT_RIGHT;
				else if (type == RoomType::TOP)
					type = RoomType::LEFT_TOP;
				else if (type == RoomType::BOTTOM_RIGHT)
					type = RoomType::LEFT_BOTTOM_RIGHT;
				else if (type == RoomType::BOTTOM_TOP)
					type = RoomType::LEFT_BOTTOM_TOP;
				else if (type == RoomType::RIGHT_TOP)
					type = RoomType::LEFT_RIGHT_TOP;
				else if (type == RoomType::BOTTOM_RIGHT_TOP)
					type = RoomType::ALL;
			}
			else if (side == 1) // add wall to bottom
			{
				if (type == RoomType::NONE)
					type = RoomType::BOTTOM;
				else if (type == RoomType::LEFT)
					type = RoomType::LEFT_BOTTOM;
				else if (type == RoomType::RIGHT)
					type = RoomType::BOTTOM_RIGHT;
				else if (type == RoomType::TOP)
					type = RoomType::BOTTOM_TOP;
				else if (type == RoomType::LEFT_RIGHT)
					type = RoomType::LEFT_BOTTOM_RIGHT;
				else if (type == RoomType::LEFT_TOP)
					type = RoomType::LEFT_BOTTOM_TOP;
				else if (type == RoomType::RIGHT_TOP)
					type = RoomType::BOTTOM_RIGHT_TOP;
				else if (type == RoomType::LEFT_RIGHT_TOP)
					type = RoomType::ALL;
			}
			else if (side == 2) // add wall to right
			{
				if (type == RoomType::NONE)
					type = RoomType::RIGHT;
				else if (type == RoomType::LEFT)
					type = RoomType::LEFT_RIGHT;
				else if (type == RoomType::BOTTOM)
					type = RoomType::BOTTOM_RIGHT;
				else if (type == RoomType::TOP)
					type = RoomType::RIGHT_TOP;
				else if (type == RoomType::LEFT_BOTTOM)
					type = RoomType::LEFT_BOTTOM_RIGHT;
				else if (type == RoomType::BOTTOM_TOP)
					type = RoomType::BOTTOM_RIGHT_TOP;
				else if (type == RoomType::LEFT_TOP)
					type = RoomType::LEFT_RIGHT_TOP;
				else if (type == RoomType::LEFT_BOTTOM_TOP)
					type = RoomType::ALL;
			}
			else if (side == 3) // add wall to top
			{
				if (type == RoomType::NONE)
					type = RoomType::TOP;
				else if (type == RoomType::LEFT)
					type = RoomType::LEFT_TOP;
				else if (type == RoomType::BOTTOM)
					type = RoomType::BOTTOM_TOP;
				else if (type == RoomType::RIGHT)
					type = RoomType::RIGHT_TOP;
				else if (type == RoomType::LEFT_BOTTOM)
					type = RoomType::LEFT_BOTTOM_TOP;
				else if (type == RoomType::BOTTOM_RIGHT)
					type = RoomType::BOTTOM_RIGHT_TOP;
				else if (type == RoomType::LEFT_RIGHT)
					type = RoomType::LEFT_RIGHT_TOP;
				else if (type == RoomType::LEFT_BOTTOM_RIGHT)
					type = RoomType::ALL;
			}
		}
		else
		{
			opens++;
			if (side == 0) // remove wall from left
			{
				if (type == RoomType::LEFT)
					type = RoomType::NONE;
				else if (type == RoomType::LEFT_BOTTOM)
					type = RoomType::BOTTOM;
				else if (type == RoomType::LEFT_RIGHT)
					type = RoomType::RIGHT;
				else if (type == RoomType::LEFT_TOP)
					type = RoomType::TOP;
				else if (type == RoomType::LEFT_BOTTOM_RIGHT)
					type = RoomType::BOTTOM_RIGHT;
				else if (type == RoomType::LEFT_BOTTOM_TOP)
					type = RoomType::BOTTOM_TOP;
				else if (type == RoomType::ALL)
					type = RoomType::BOTTOM_RIGHT_TOP;
			}
			else if (side == 1) // remove wall from bottom
			{
				if (type == RoomType::BOTTOM)
					type = RoomType::NONE;
				else if (type == RoomType::LEFT_BOTTOM)
					type = RoomType::LEFT;
				else if (type == RoomType::BOTTOM_RIGHT)
					type = RoomType::RIGHT;
				else if (type == RoomType::BOTTOM_TOP)
					type = RoomType::TOP;
				else if (type == RoomType::LEFT_BOTTOM_RIGHT)
					type = RoomType::LEFT_RIGHT;
				else if (type == RoomType::LEFT_BOTTOM_TOP)
					type = RoomType::LEFT_TOP;
				else if (type == RoomType::BOTTOM_RIGHT_TOP)
					type = RoomType::RIGHT_TOP;
				else if (type == RoomType::ALL)
					type = RoomType::LEFT_RIGHT_TOP;
			}
			else if (side == 2) // remove wall from right
			{
				if (type == RoomType::RIGHT)
					type = RoomType::NONE;
				else if (type == RoomType::LEFT_RIGHT)
					type = RoomType::LEFT;
				else if (type == RoomType::BOTTOM_RIGHT)
					type = RoomType::BOTTOM;
				else if (type == RoomType::RIGHT_TOP)
					type = RoomType::TOP;
				else if (type == RoomType::LEFT_BOTTOM_RIGHT)
					type = RoomType::LEFT_BOTTOM;
				else if (type == RoomType::LEFT_RIGHT_TOP)
					type = RoomType::LEFT_TOP;
				else if (type == RoomType::BOTTOM_RIGHT_TOP)
					type = RoomType::BOTTOM_TOP;
				else if (type == RoomType::ALL)
					type = RoomType::LEFT_BOTTOM_TOP;
			}
			else if (side == 3) // remove wall from top
			{
				if (type == RoomType::TOP)
					type = RoomType::NONE;
				else if (type == RoomType::LEFT_TOP)
					type = RoomType::LEFT;
				else if (type == RoomType::BOTTOM_TOP)
					type = RoomType::BOTTOM;
				else if (type == RoomType::RIGHT_TOP)
					type = RoomType::RIGHT;
				else if (type == RoomType::LEFT_BOTTOM_TOP)
					type = RoomType::LEFT_BOTTOM;
				else if (type == RoomType::LEFT_RIGHT_TOP)
					type = RoomType::LEFT_RIGHT;
				else if (type == RoomType::BOTTOM_RIGHT_TOP)
					type = RoomType::BOTTOM_RIGHT;
				else if (type == RoomType::ALL)
					type = RoomType::LEFT_BOTTOM_RIGHT;
			}
		}
	}

	float Room::weight()
	{
		return get_breadth_heuristic();// +DistanceToGoal(Location::goal);
	}

	std::vector<bool> Room::getOpenings()
	{
		return{ openings[0]->getClosed(), openings[1]->getClosed(), openings[2]->getClosed(), openings[3]->getClosed() };
	}

	int Room::get_breadth_heuristic()
	{
		return breadth_heuristic;
	}

	void Room::set_breadth_heuristic(int new_value)
	{
		breadth_heuristic = new_value;
	}

	void Room::reset_breadth_heuristic()
	{
		breadth_heuristic = std::numeric_limits<int>::max();
	}

	//void Room::set_adjacent_rooms(Room(&maze)[16][16])
	//{
	//	// left
	//	if (loc.x - 1 > 0)
	//		neighbors.push_back(maze[loc.y][loc.x - 1]);
	//
	//	// top
	//	if (loc.y + 1 < 16)
	//		neighbors.push_back(maze[loc.y + 1][loc.x]);
	//
	//	// right
	//	if (loc.x + 1 < 16)
	//		neighbors.push_back(maze[loc.y][loc.x + 1]);
	//
	//	// bottom
	//	if (loc.y - 1 > 0)
	//		neighbors.push_back(maze[loc.y - 1][loc.x]);
	//}

	std::vector<Room*> * Room::get_children()
	{
		std::vector<Room*> * children = new std::vector<Room*>();

		for (auto wall : openings)
		{
			if (wall->getClosed() == false)	// if wall is open
				children->push_back(wall->getOtherParent(this));
		}

		return children;
	}

	/*
	Get children that are reachable and not the previous room
	*/
	std::vector<Room*> * Room::get_children(Room * previous)
	{
		std::vector<Room*> * children = new std::vector<Room*>();

		if (previous == nullptr)
			return children;

		for (auto wall : openings)
		{
			if (wall->getClosed() == false)	// if wall is open
			{
				Room * otherParent = wall->getOtherParent(this);
				if ((otherParent->Location().x != previous->Location().x) || (otherParent->Location().y != previous->Location().y))
					children->push_back(otherParent); // add if this isn't the previous cell
			}
		}

		return children;
	}


	/* Get children that aren't already in the given collection */
	std::vector<Room*> * Room::get_children(std::vector<Room*> & existingCollection)
	{
		std::vector<Room*> * children = new std::vector<Room*>();

		for (auto wall : openings)
		{
			bool existsInOtherCollection = false;

			// If wall is known to be open, or is simply unknown
			if ((wall->known >= 0 && wall->getClosed() == false) || wall->known < 0)
			{
				for (int i = 0; i < existingCollection.size(); i++)
					if (wall->getOtherParent(this) == existingCollection[i])
						existsInOtherCollection = true;

				if (!existsInOtherCollection)	// add child if it's not already in the given collection
					if (wall->getOtherParent(this) != nullptr)
						children->push_back(wall->getOtherParent(this));
			}
		}

		return children;
	}

	// Get confidence of the walls for the specified path. 
	// If edgeConfidence, return _alongPathConfidence.
	// -100: error. values weren't calculated
	int Room::confidence(int pathNumber, bool edgeConfidence)
	{
		if (!edgeConfidence)	// walls in path
		{
			// Make sure the values were calculated
			if (_inPathConfidence.count(pathNumber) == 0)
				return -100;

			return _inPathConfidence[pathNumber];
		}

		// walls along path *******************

		// Make sure the values were calculated
		if (_alongPathConfidence.count(pathNumber) == 0)
			return -100;

		return _alongPathConfidence[pathNumber];

	}

	void Room::reset()
	{
		checked.clear();
		previous.clear();
		next.clear();
		_alongPathConfidence.clear();
		_inPathConfidence.clear();
	}

	// Return 1 if there's a turn or 0 if there isn't
	int Room::turn(int pathNumber)
	{
		if (previous.count(pathNumber) == 0 || next.count(pathNumber))
			return 0;	// this might be a mistake, but 

		if (previous[pathNumber]->Location().x != next[pathNumber]->Location().x)
			return 1;
		if (previous[pathNumber]->Location().y != next[pathNumber]->Location().y)
			return 1;

		return 0;
	}

	void Room::operator=(Room & room2)
	{
		this->breadth_heuristic = room2.breadth_heuristic;
		this->checked = room2.checked;
		this->loc = room2.loc;
		this->neighbors = room2.neighbors;
		this->openings = room2.openings;
		this->opens = room2.opens;
		this->type = room2.type;
		this->_alongPathConfidence = room2._alongPathConfidence;
		this->_inPathConfidence = room2._inPathConfidence;
	}

	bool Room::operator<(Room & room2)
	{
		if (this->weight() < room2.weight())
			return true;
		return false;
	}

	bool Room::operator>(Room & room2)
	{
		if (this->weight() > room2.weight())
			return true;
		return false;
	}

	bool Room::operator==(Room & room2)
	{
		if (this->weight() == room2.weight())
			return true;
		return false;
	}

	bool Room::operator<=(Room & room2)
	{
		if (this->weight() <= room2.weight())
			return true;
		return false;
	}

	bool Room::operator>=(Room & room2)
	{
		if (this->weight() >= room2.weight())
			return true;
		return false;
	}

}