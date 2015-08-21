#ifndef __CELL_NEW_H__
#define __CELL_NEW_H__

#include <iostream>
#include <vector>
#include "Coordinate_new.h"
#include "Direction.h"
#include "Neighbors.h"

namespace Algorithm
{
	/**
	* The cell class is contained by the Grid class.  A cell contains publicly accessible
	* pointers to reachable adjacent cells.  There also exists hidden points to adjacent cells,
	* which always exist only for the Grid class to access.
	* Adjacent cells are to be set to nullptr once it's determined that there's a wall between them
	*/
	class Cell_new
	{
	public:
		Cell_new();
		Cell_new(int x, int y);
		~Cell_new();

		// Public coordinate getter
		Coordinate_new * get_coordinate(){ return coordinate; }


		Cell_new * get_neighbor(Direction direction);				// Adjacent cell getter
		void set_neighbor(Direction direction, Cell_new * cell);	// Adjacent cell setter

		// Return the collection of adjacent cells from the perspective of the given parent cell
		// Collection does not include the parent
		std::vector<Cell_new*> * children();
		std::vector<Cell_new*> * children(Cell_new * _parent);

		// Set the parent cell, which is used during searches
		void set_parent(Cell_new * _parent) { parent = _parent; }
		Cell_new * get_parent() { return parent; }

		void print();				// debugging use
		void print_breadth_heuristic();

		void set_breadth_heuristic(int new_value);
		int get_breadth_heuristic();
		

	private:
		friend class Grid_new;		// Give Grid class special access

		Coordinate_new *coordinate;

		// Publicly accessible adjacent cell pointers that can change
		Neighbors<Cell_new*> *neighbors_accessible;

		// Adjacent cell pointers that don't change
		Neighbors<Cell_new*> *const_neighbors;

		// Parent cell, used during searches
		Cell_new * parent;

		int breadth_heuristic;
		void reset_breadth_heuristic();
	};

	Cell_new::Cell_new()
	{
		coordinate = new Coordinate_new();
		neighbors_accessible = new Neighbors<Cell_new*>();
		const_neighbors = new Neighbors<Cell_new*>();
		parent = nullptr;
		breadth_heuristic = std::numeric_limits<int>::max();
	}

	Cell_new::Cell_new(int x, int y)
	{
		coordinate = new Coordinate_new(x, y);
		neighbors_accessible = new Neighbors<Cell_new*>();
		const_neighbors = new Neighbors<Cell_new*>();
		parent = nullptr;
		breadth_heuristic = std::numeric_limits<int>::max();
	}

	Cell_new::~Cell_new()
	{
		delete(coordinate);
		delete(neighbors_accessible);
		delete(const_neighbors);
	}

	/**
	* Return the specified cell
	*/
	Cell_new * Cell_new::get_neighbor(Direction direction)
	{
		return neighbors_accessible->getNeighbor(direction);
	}

	
	void Cell_new::set_neighbor(Direction direction, Cell_new * cell)
	{
		neighbors_accessible->setNeighbor(direction, cell);
	}

	/**
	* Return the collection of adjacent cells from the perspective of the given parent cell
	* Collection does not include the parent
	*/
	std::vector<Cell_new*> * Cell_new::children(Cell_new * _parent)
	{
		set_parent(_parent);
		return children();
	}

	/**
	* Return the collection of adjacent cells from the perspective of the given parent cell
	* Collection does not include the parent
	*/
	std::vector<Cell_new*> * Cell_new::children()
	{
		std::vector<Cell_new*> *vect =  new std::vector<Cell_new*>({ get_neighbor(Direction::B), get_neighbor(Direction::L), get_neighbor(Direction::R), get_neighbor(Direction::T) });
		std::vector<Cell_new*>::iterator vi = vect->begin();
		int i = 0;
		while (vi != vect->end())
		{
			if ((*vi) == parent)
			{
				vect->erase(vect->begin() + i);
				break;
			}
			vi++;	i++;
		}
		return vect;
	}

	/**
	* Print the cell's walls if they exist, and the cell's coordinates
	*/
	void Cell_new::print()
	{
		if (this->get_neighbor(Direction::L) == nullptr)		// left wall
			std::cout << "|";
		else
			std::cout << " ";
		if (this->get_neighbor(Direction::T) == nullptr)		// top wall
			std::cout << "*";
		else
			std::cout << " ";
		if (this->coordinate != nullptr)
		{
			std::cout << " ";
			this->coordinate->print();	// coordinate
			std::cout << " ";
		}
		if (this->get_neighbor(Direction::B) == nullptr)	// bottom wall
			std::cout << "_";
		else
			std::cout << " ";
		if (this->get_neighbor(Direction::R) == nullptr)		// right wall
			std::cout << "|";
		else
			std::cout << " ";
	}

	void Cell_new::print_breadth_heuristic()
	{
		std::cout << " " << breadth_heuristic << " ";
	}

	void Cell_new::set_breadth_heuristic(int new_value)
	{
		breadth_heuristic = new_value;
	}

	int Cell_new::get_breadth_heuristic()
	{
		return breadth_heuristic;
	}

	void Cell_new::reset_breadth_heuristic()
	{
		breadth_heuristic = std::numeric_limits<int>::max();
	}
}
#endif