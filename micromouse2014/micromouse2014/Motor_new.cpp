#include "Motor_new.h"

namespace Hardware
{
	void Motor::motion(MotorMotion motion)
	{
		switch (motion)
		{
		case MotorMotion::TURN_LEFT: 
			break;
		case MotorMotion::TURN_RIGHT: 
			break;
		case MotorMotion::TURN_AROUND:
			break;
		case MotorMotion::MOVE_FORWARD:
			break;
		case MotorMotion::MOVE_BACKWARD: // MOVE_BACKWARD
			break;
		default:
			return;
		}
	}
}