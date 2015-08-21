#ifndef __MOUSE_H__
#define __MOUSE_H__

// Delay
#include <chrono>
#include <thread>

#include <deque>
#include <memory>

#include "Direction.h"
#include "Filter.h"
#include "Motor_new.h"
#include "Room.h"
#include "Searches.h"
#include "Sensor.h"

namespace Algorithm
{
	class Mouse
	{
		Data::Direction direction;
		std::unique_ptr<Data::Maze> maze;
		std::unique_ptr<Hardware::Sensor> sensor;
		std::unique_ptr<Hardware::Motor> motor;
		Data::Room * currentRoom;

		/* Set the initial direction based on the random starting position */
		void InitialDirection();

		// Add or Remove from the actual path depending on if mouse is backtracking
		bool backtracking;
	public:
		Mouse();
		~Mouse();		

		void Explore();
		std::unique_ptr<Path_new> * Evaluate();
		
		// Turn to the path's front room and add the path to the current path
		void Turn(Path_new * newPath);

		void Turn(Data::Room * room);

		void TurnAround();

		void MoveForward();
		

		// Getters

		Data::Location * location();

	private:
		// Check to see if there's more than one new path for the current physical room
		int CheckForOptions();

		/* Generate paths, return best path */
		std::unique_ptr<Path_new> * GeneratePaths();

		// Path that includes the actual path + the current best-guess to the goal
		Path_new * completePathGuess;

		/* Path that the mouse has actually taken */
		Path_new * actualPath;

		Data::Room * previousRoom;

		std::deque<Data::Room*> visitedRooms;
	};	
}

#endif