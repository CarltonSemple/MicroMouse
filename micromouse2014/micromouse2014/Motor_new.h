#pragma once
#ifndef __MOTOR_H__
#define __MOTOR_H__

namespace Hardware
{
	enum MotorMotion
	{
		TURN_LEFT,
		TURN_RIGHT,
		TURN_AROUND,
		MOVE_FORWARD,
		MOVE_BACKWARD
	};

	class Motor
	{
	public:
		void motion(MotorMotion motion);
	};
}
#endif