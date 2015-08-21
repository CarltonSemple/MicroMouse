#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <deque>
#include <memory>
#include "ExploreMode.h"
#include "Path_new.h"
#include "Room.h"
#include "Sorts.h"

namespace Algorithm
{
	class Filter
	{
	public:
		static ExploreMode mode;
		static std::unique_ptr<Path_new> * bestPath(std::deque<std::unique_ptr<Path_new>> *paths)
		{
			switch (Filter::mode)
			{
			case ExploreMode::FarReaching:
				Path_new::mode = ExploreMode::FarReaching;
				break;
			case ExploreMode::Direct:
				Path_new::mode = ExploreMode::Direct;
				break;
			default:
				return nullptr;
			}

			//Sorting::Sorts<std::unique_ptr<Path_new>>::quicksort(*paths, 0, paths->size()-1);
			
			// Merge sort iteratively
			Sorting::SortingT<std::deque<std::unique_ptr<Path_new>>>::mergesort_i(*paths);

			std::unique_ptr<Path_new> & returnedPath = paths->front();

			return &returnedPath;
		}

		// Returns the better path
		static Path_new * BetterPath(Path_new * p1, Path_new * p2, Data::Location &goal)
		{
			if (p1 == nullptr || p2 == nullptr)
				return nullptr;

			Path_new * chosenPath;
			int p1_wins = 0, p2_wins = 0;

			// Distance to goal
			if (p1->Rooms()->back()->DistanceToGoal(goal) < p2->Rooms()->back()->DistanceToGoal(goal))	//((p1->averageDistanceToGoal(goal) + p1->_rooms->back()->DistanceToGoal(goal)) < (p2->averageDistanceToGoal(goal) + p2->_rooms->back()->DistanceToGoal(goal)))
			{
				if (p1->Rooms()->size() < p2->Rooms()->size())
					p1_wins++;
			}

			if (p1_wins > p2_wins)
				return p1;
			else
				return p2;

			return nullptr;
		}

		/*
		Return pointer to the room with the least visits in a given collection
		*/
		template <typename SequenceContainer>
		static Data::Room * LeastVisited(SequenceContainer collection)
		{
			if (collection->size() == 0)
				return nullptr;

			int equalCount = 1;

			Data::Room * lowest = collection->front();
			for (int i = 1; i < collection->size(); i++)
			{
				Data::Room * cu = collection->at(i);
				if (cu->visits < lowest->visits)
					lowest = cu;
				else if (cu->visits == lowest->visits)
					equalCount++;
			}

			return lowest;
		}
	};	
}
#endif