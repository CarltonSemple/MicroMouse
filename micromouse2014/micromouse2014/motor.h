//
//  motor.h
//  Test
//
//  Created by Lyle Moffitt on 3/9/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//
#ifndef __Test__motor__
#define __Test__motor__

#include <iostream>
//#include "device.h"
#include "gpio.h"
#include "encoder.h"

//#include "device.cpp"


class motor;
class encoder;

namespace motor_config
{
	static auto path_ls =
	{
		"/sys/devices/ocp.3/pwm_test_45.11/",//The base bath for motor1
		"/sys/devices/ocp.3/pwm_test_46.13/" //The base bath for motor2
	};

	static auto attr_ls =
	{
		"period",
		"duty",
		"polarity",
		"run"
	};

	constexpr    uint max_period = 10000000;
	constexpr    uint max_speed = max_period;

	enum m_select
	{
		LEFT = 0,
		RIGHT = 1
	};

};

class motor
{
	device_dir mtr_dev;//, enc_dev;
	gpio enabler;

protected:
public:
#define make_getter_setter(name, type, i)           \
type name(){    return mtr_dev[#name].gt<i>();  }   \
void name(type val){   mtr_dev[#name].st(val);  }   \

	make_getter_setter(period, int, int)
		make_getter_setter(duty, int, int)
		make_getter_setter(polarity, int, int)
		make_getter_setter(run, int, int)

#undef  make_getter_setter



public:


	//template<typename... Args> motor(Args&&... args):mtr_dev( args... ){}

	motor(motor_config::m_select side) :
		mtr_dev(motor_config::path_ls.begin()[side],
		motor_config::attr_ls),
		enabler(side)
	{}

	motor(device_dir _m, gpio _e) : mtr_dev(_m), enabler(_e)
	{}

	// Set speed as ratio of maximum
	void set_speed(double _val);

	// Get speed of motor as ratio of maximum
	double get_speed();

	// Turn the motor on ; apply voltage 
	void forward();

	// Turn the motor off; cost
	void backward();

	// Check to see if the motor is enabled (true) or disabled
	bool chk_en();



};


#endif /* defined(__Test__motor__) */