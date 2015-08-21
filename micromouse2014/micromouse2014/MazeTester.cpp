#include "Maze.h"
#include "Motor_new.h"
#include "Sensor.h"
#include "Display.h"

///<summary>
/// Method solveMaze
/// This method solves the maze using a simple
/// random choice algorithm where the next
/// room to move to is randomly chosen each time
/// a room is entered. The method also prints
/// out the progress being made as the maze
/// is explored.
/// </summary>
void solveMaze(Maze maze)
{
	Location *current = maze.Start();
	std::array<Room*, 4> goal = maze.Goal();
	Room* currentRoom = maze(current->x, current->y);

	int leftTurns = 0, rightTurns = 0, aboutTurns = 0, forward = 0, backward = 0;
	Motor motor;
	Sensor sensor;
	Display display(maze);

	int random = 0;

	bool done = false;
	/*INSERT ALGORITHM BELOW*/
	while (!done)
	{
		random = rand() % 4; // randomly chooses which way to go
		while (currentRoom->getWalls()[random]->getClosed())
			random = rand() % 4;
		while (maze.Direction() != random) // make turns
		{
			if (std::abs(maze.Direction() - random) == 3)
			{
				if (random == 0) // want to go West (0) and facing North (3)
				{ motor.motion(MotorMotion::TURN_LEFT); maze.Direction(maze.Direction() + 1); leftTurns++; }
				else // want to go North (3) and facing West (0)
				{ motor.motion(MotorMotion::TURN_RIGHT); maze.Direction(maze.Direction() - 1); rightTurns++; }
			}
			else if (std::abs(maze.Direction() - random) == 2) // turn around
			{
				motor.motion(MotorMotion::TURN_ABOUT);
				if (maze.Direction() != 3)
					maze.Direction(maze.Direction() + 2);
				else
					maze.Direction(maze.Direction() - 2);
				aboutTurns++;
			}
			else // turn whichever way is closer
			{
				if (maze.Direction() < random)
				{ motor.motion(MotorMotion::TURN_LEFT); maze.Direction(maze.Direction() + 1); leftTurns++; }
				else
				{ motor.motion(MotorMotion::TURN_RIGHT); maze.Direction(maze.Direction() - 1); rightTurns++; }
			}
		}

		// After making the turns, move
		// At the moment, I tell it to always turn until it can move forward
		motor.motion(MotorMotion::MOVE_FORWARD);
		if (random == 0) // moving to room to the left
			current->y--;
		else if (random == 1) // moving to room below
			current->x++;
		else if (random == 2) // moving to room to the right
			current->y++;
		else // moving to room above
			current->x--;
		forward++;

		// Scan surroundings
		//Maze* _readMaze = sensor.read();

		// Set walls to known
		maze.Current(current);
		currentRoom = maze(current->x, current->y);
		std::vector<Wall*> walls = currentRoom->getWalls();
		for (int i = 0; i < 4; i++)
			walls[i]->known++;

		// Print out progress
		display.print();
		std::cout << "\n\n";

		// check if we're at the goal
		for (int i = 0; i < 4; i++)
			if ((*current) == goal[i]->Location())
				done = true;
	}
	std::cout << "\n\nLeft Turns: " << leftTurns << "\nRight Turns:" << rightTurns << "\nAbout Turns: " << aboutTurns << "\nForward Motions: " << forward;
}

/*****TEST THE MAZE*****/
void main()
{
	Maze m;
	m.printMaze(false);
	//solveMaze(m);
	getchar();
}