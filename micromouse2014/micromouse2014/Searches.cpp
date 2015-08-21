#include "Searches.h"
#include "Sorts.h"

//#include "Cell_new.h"
using namespace Data;

namespace Algorithm
{
	/**
	* Update the cells' heuristic values starting from the given origin cell
	*/
	void Searches::breadth_first_search(Maze &maze, int x, int y)
	{
		std::queue<Room*> * q = new std::queue<Room*>();	// queue holds child cells

		// Set all the breadth heuristics for the cells to the max integer size
		maze.resetBreadthHeuristics();

		Room * current_room = maze(x, y);

		current_room->set_breadth_heuristic(0);	// start heuristic count at 0

		do
		{
			// add children to queue

			// get reachable child cells
			std::vector<Room*> * children = current_room->get_children();

			std::vector<Room*>::iterator cIt = children->begin();
			while (cIt != children->end())
			{
				// Do each node only once
				//if ((*cIt)->get_breadth_heuristic() == std::numeric_limits<int>::max())
				if (current_room->get_breadth_heuristic() + 1 < (*cIt)->get_breadth_heuristic())
				{
					(*cIt)->set_breadth_heuristic(current_room->get_breadth_heuristic() + 1);   // set heuristic to parent's heuristic + 1
					q->push((*cIt));                              // add to queue
				}
				
				++cIt;
			}
			delete(children);

			if (q->size() == 0)
				break;
			current_room = q->front();		// move to next child
			q->pop();
		} while (true);
	}

	/*
	std::deque<Path_new*> * Searches::depth_search(Maze &maze, Location &start, Location &goal)
	{
		// Reset the path count and the rooms' checked values
		Path_new::resetPathCount();
		maze.clearChecked();

		std::deque<Path_new*> *paths = new std::deque<Path_new*>();	// set of paths to return

		// Temporarily hold rooms and the paths to each 
		std::stack<Room*> * stac = new std::stack<Room*>();
		std::stack<Path_new*> * pstack = new std::stack<Path_new*>();

		// Starting room & path
		Room * current_room = maze(start.x, start.y);
		Path_new * current_path = new Path_new(current_room);

		do
		{
			// Mark the room as checked by the current path
			current_room->checked[current_path->Number()] = true;

			// get reachable child cells that aren't already in the path
			std::vector<Room*> * children = current_room->get_children(*current_path->RoomsVector());

			// Sort the children according to their weight (greatest is first)??????
			Sorting::Sorts<Room*>::quicksort(*children, 0, children->size()-1);
			//Sorting::Sorts<Room*>::reverse(*children);

			// Set childrens' pointers to current room as the previous room for the current path
			

			// Add children to stack
			int newPaths = 0;	          // number of paths branching from current one
			int childCount = children->size();
			while (children->size() > 0)
			{
				std::vector<Room*>::reverse_iterator nIt = children->rbegin();
				auto n_child = *nIt;

				// Add the child if the current path hasn't already looked at it
				if ((n_child->checked.count(current_path->Number()) == 0) 
					//&& n_child != current_room->previous[current_path->Number()]
					//&& n_child->checked[current_path->Number()-1] == false) // check if previous path saw it
					)
				{
					n_child->checked[current_path->Number()] = true;

					// Calculate the inner & outer wall confidence of the paths while building
					//Room::MarkPathAndCalculateWallCosts(current_path->Number(), current_room, n_child);

					stac->push(n_child);	// Push the child with the least weight/confidence first

					// only push a new path if there was a branch
					if (childCount > 1)
						pstack->push(new Path_new(current_path, n_child));
					newPaths++;
				}				

				children->pop_back();
			}

			// Add path to list of paths if there wasn't anything left
			if (current_path->Contains(goal.x, goal.y))
				paths->push_back(current_path);

			// Look at next room & path
			if (stac->size() == 0)
				break;
			
			//current_room = stac->top();
			//current_path = pstack->top();
			//stac->pop();
			//pstack->pop();
			do
			{
				current_room = stac->top();
				stac->pop();

				if (pstack->size() > 0)
				{
					current_path = pstack->top();
					pstack->pop();
				}
								
				if (!current_path->Contains(goal.x, goal.y))
					break;
				else
					paths->push_back(current_path);
			} while (stac->size() > 0);

			

		} while (true);
		

		return paths;
	}
	*/


	Path_new * Searches::depth_search(Maze &maze, Path_new *actualPath, Location &start, Location &goal)
	{
		// Reset the path count and the rooms' checked values
		Path_new::resetPathCount();
		maze.clearChecked();

		std::deque<Path_new*> *paths = new std::deque<Path_new*>();	// set of paths to return

		// Starting room & path
		Room * current_room = maze(start.x, start.y);
		Path_new * current_path = new Path_new(actualPath); //(current_room);

		// set of stacks corresponding to the paths
		std::map<int, std::stack<Room*>> pathStacks;

		// Set up first stack & add create/add path from the actual path
		paths->push_back(current_path);
		std::stack<Room*> & st = pathStacks[current_path->Number()];
		st.push(current_room);		
		
		// Best path
		Path_new * bestPath = current_path;

		int success = 0;
		while(success != 1)
			success = depth_helper(pathStacks, *paths, &bestPath, goal);

		//pathStacks.clear();
		//delete pathStacks;

		return bestPath; // return best path
	}

	int Searches::depth_helper(std::map<int, std::stack<Data::Room*>> & pathStacks, std::deque<Path_new*> & paths, Path_new **bestPath, Data::Location & goal)
	{
		//std::map<int, std::stack<Room*>>::iterator sIt = pathStacks.begin();
		//std::deque<Path_new*>::iterator pIt = paths.begin();

		int finishedCount = 0;
		int successCount = 0;

		// For each path
		for (int i = 0; i < paths.size(); i++)	// safer for adding to collection
		{
			bool pathCreated = false;

			if (paths[i] == nullptr)
				continue;

			Path_new *current_path = paths[i];
			// Unsuccessful path
			if (pathStacks[current_path->Number()].size() == 0)
			{
				finishedCount++;
				current_path->finished = true;
				// Delete the unsuccessful path
				//delete current_path;
				//paths[i] = nullptr;
				
				continue;
			}

			// Successful path
			if (current_path->success)
			{
				//if (current_path->finished == false)
					//successful_paths.push_back(current_path);
				if (current_path->finished)
					continue;
				current_path->finished = true;
				finishedCount++;
				successCount++;
				continue;
			}

			// Update the best path
			*bestPath = Filter::BetterPath(current_path, *bestPath, goal);

			std::stack<Room*> & stackCurrent = pathStacks[current_path->Number()];
			Room * current_room = stackCurrent.top();
			stackCurrent.pop();

			Location & currentSpot = current_room->Location();
			if (currentSpot.x == goal.x && currentSpot.y == goal.y)
			{
				finishedCount++;
				current_path->success = true;
				continue;
			}
			
			// get reachable child cells that aren't already in the path
			std::vector<Room*> * children = current_room->get_children(*current_path->RoomsVector());
			Sorting::Sorts<Room*>::quicksort(*children, 0, children->size() - 1); // sort according to weight
			
			Room * n_child;

			// Create new paths if need be
			while (children->size() > 1)
			{			
				std::vector<Room*>::reverse_iterator nIt = children->rbegin();
				n_child = *nIt;

				// Calculate the inner & outer wall confidence of the paths while building
				Room::MarkPathAndCalculateWallCosts(current_path->Number(), current_room, n_child);

				Path_new *newpath = new Path_new(current_path, n_child);
				paths.push_back(newpath);

				// set up new stack for the new path
				pathStacks[newpath->Number()].push(n_child);	

				children->pop_back();

				pathCreated = true;
			}

			// Keep current path and expand it with the (last) new child
			if (children->size() > 0)
			{
				std::vector<Room*>::reverse_iterator nIt = children->rbegin();
				n_child = *nIt;
				
				// Calculate the inner & outer wall confidence of the paths while building
				Room::MarkPathAndCalculateWallCosts(current_path->Number(), current_room, n_child);

				pathStacks[current_path->Number()].push(n_child);
				children->pop_back();
				current_path->Rooms()->push_back(n_child);	// expand current path
			}
			delete(children);

			//if (pathCreated)
				//i = 0;      // restart at beginning to give every possible path a fair share

			// end after 150,000
			if (paths.size() > 499) //10000)
			{
				return 1;
			}
		}

		if (finishedCount >= paths.size())
			return 1;

		return 0;
	}

	void Searches::replacePathInCollection(std::deque<Path_new*> &paths, Path_new * newPath)
	{

	}
}