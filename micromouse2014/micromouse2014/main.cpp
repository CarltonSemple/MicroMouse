//
//  main.cpp
//  micromouse2014
//
// Created by Lyle Moffitt, Carlton Semple, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

//#include "robot.h"
//#include "device.h"

//#include "gpio.h"

//#include "../test.h"

//#include "motor.cpp"    // MUST BE .cpp DO NOT USE .h
//#include "gpio.h"     // When using ONLY gpio

//#include "lidar.h"      // When using lidar

//#include "motor.h"

//#include "nav.h"
//#include "star.h"

#include "robot.h"

#include <iostream>
#include <string>

using namespace std;

/*
namespace test
{
    static auto path_ls = 
    {
        "/Users/Lyle/Desktop/test1/",
        "/Users/Lyle/Desktop/test2/"
    };
    
    static auto attr_ls =
    {
        "t1",
        "t2"
    };
}
*/

int main(int argc, const char * argv[])
{   
    robot rob;
    
    /* At the very least, the constructor works; needs testing
    lidar _l (string(test::path_ls.begin()[0]) + "t1");
    */
    /* Probably works.. needs to be tested on live /dev/tty 
    device_tty _t(string(test::path_ls.begin()[0])+"tty");
    _t.wr_("This is a string");
    auto _ret = _t.rd_<string>(4);
    cout<< _ret <<" is a "<<typeid(_ret).name() << endl;
    */
    /*
    motor _m (device_dir(test::path_ls.begin()[0] , 
                         motor_config::attr_ls) ,
              gpio(test::path_ls.begin()[1] , 
                   gpio_config::attr_ls)                    );
    
    _m.duty(4321);
    _m.enable();
	cout << _m.duty() << endl;
    */
   
    /* Testing just gpio
    gpio dd ( test::path_ls.begin()[0] , gpio_config::attr_ls);
     
    dd.direction("hello world");
    string ret = dd.direction();
     
    cout<<ret<<endl;
    */
    
    
    
    return 0;
}

// #define LIDAR_TESTING
#ifdef LIDAR_TESTING

#include <iostream>
#include <map>

#include "lidar.cpp"

using namespace std;

int main(int argc, char ** argv )
{
	string str ;
	for( int i =1; i < argc; ++i){ str.append( string(argv[i]) );	}
	int index = strtol(str.c_str(),nullptr,10);
	lidar _l;
	map<timestamp, _360_scan*>::iterator it;
	
	while(true)
	{
		_l.build_scan()->at(index)->source->print();
	}
	
	return 0;
}

#endif

