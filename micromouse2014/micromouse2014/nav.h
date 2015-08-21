//
//  nav.h
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//

#ifndef _micromouse__nav_
#define _micromouse__nav_


#include "motor.h"
#include "encoder.h"

#include "lidar.h"
#include "packet.h"

#include <iostream>
#include <tuple>

class star;
class nav;

namespace nav_config
{
    constexpr double PI             = 3.14159265359;
    
	constexpr double max_speed      = 1; 
	constexpr double min_speed      = 0;
	constexpr double min_negspeed   = 0;
	constexpr double max_negspeed   = 1;
	constexpr double max_invariance = 0.0001;
	constexpr double min_invariance = 0.00000001;
	constexpr size_t duration       = 10000000;
    
    //Wheels
    constexpr uint wheel_diameter   = 42;//mm
    constexpr uint unit_per_rot     = 45;
    constexpr uint wheel_base       = 80;//mm
    
    constexpr double std_move_speed = 0.5;
    
    constexpr uint cell_size        = 180;//mm
    
	// for straight sleep
	constexpr size_t sleeptime      = 100; // microseconds

	// returns true if they are similar
    static inline
    bool eqish(double a, double b);

}
//change

class nav
{
    friend star;
private:
	motor   left; 
	motor   right;
    
    encoder& enc;
    
    lidar&  view;

protected:
    inline //-1 is invalid
    int dist_at(uint degree)
    {
        auto it = view.scan_hist.front()->at(degree);
        return (it->invalid_data?-1:it->distance);
    }
    
public:
    /* ____Primary methods:____ require specific arguments */
    
    // Class constructor for navigation
    nav(lidar & lidar_ptr, encoder & encoder_ptr);

    // moves a distance in terms of mm 
	void movedistancevariable(int mm);
    
	// makes sure wheels are moving in step at same speed 
    bool synchronize(double speed);
	 
	// assuming the front middle is 0 degrees; positive == clockwise 
	void turn(double angle);

    // move at constant velocity
    void move(double speed);
    
    /** veers the rover to the left based on ratio of speeds
     Positive:  right is increased relative to left
     Negative:  left is decreased relative to right
     */
	void veerleft(double ratio);
	
	/** veers the rover to the right based on ratio of speeds
     Positive:  left is increased relative to right
     Negative:  right is decreased relative to left
     */
	void veerright(double ratio);
    
    
    /* ____Secondary methods:____ call primaries with constant values */
    
	// go forward one unit cell and stop
	void goForwardOne();
	
	// turns rover 90 deg to the left (static)
    inline
	void turnleft(){    turn(-90);  }
	
	// turns rover 90 deg to the right  (static)
    inline
	void turnright(){   turn(90);   }
	
	//spins the rover around 180 deg
    inline
	void turnaround(){  turn(180);  }
    
	//stops motor 
	void stop(){ move(0); }


	// keep the robot facing straight - in parallel with the walls along the sides of it
	void stayStraightLoop();
    
	
	/** the following each return true if they find that the robot isn't going straight**/
	void straightAhead(   int &leftORright);
	void oneSidedApproach(int &leftORright);
	void twoSidedApproach(int &leftORright);

	// solve for max accepted distance for this specific angle
	void maxAcceptedDistance(int angle, double &acceptedDistance);

	// used for threads
	int straightLR, oneLR, twoLR;	// -1 means left is closer... 1 means right is closer
	bool straightVal, oneSideVal, twoSideVal;
		
};





#endif //_micromouse__nav_

