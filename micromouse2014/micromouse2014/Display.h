#pragma once
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Maze.h"

class Display
{
public:
	Display();
	Display(Data::Maze m) { maze = m; }
	void print(); // print info
	void collect(); // collect will gather all information from Sensor and Motor and update Maze
private:
	Data::Maze maze;
};

#endif