//
//  nav.cpp
//
// Created by Carlton Semple, Lyle Moffitt, & Nick Beni
// Created at Syracuse University for the 2014 micromouse competition
//


#include "nav.h"
#include "macros.h"
#include <cmath>

using namespace std;

bool
nav_config::eqish(double a, double b)
{
    return (ABS(DIV(a,b))-1<=(0+min_invariance) &&
            ABS(DIV(a,b))-1<=(0-min_invariance) ? true:false); 
}


// Defualt constructor
nav::nav(lidar & lidar_ptr, encoder & encoder_ptr):
view(lidar_ptr),
enc(encoder_ptr),
left(motor_config::m_select::LEFT),
right(motor_config::m_select::RIGHT)
{
    
}


void 
nav::movedistancevariable(int mm)
{
    using namespace encoder_config;
    using namespace nav_config;
    
    stop(); // begin from stopped position
    if (mm==0) {    return;     }
    
    snapshot *_t, *_t0 = enc.hist.front();
//    int _l , _l0 = dist_at(lidar_config::degree_north);
    auto goal = (mm * DIV((PI*wheel_diameter),unit_per_rot));
    
#define avg_diff                                        \
    AVG(( get<1>(*_t)->_pos - get<1>(*_t0)->_pos ),     \
        ( get<2>(*_t)->_pos - get<2>(*_t0)->_pos ))
    
    move( std_move_speed*(mm>0?1:   mm<0?-1  :0) );
    
    double _d = 1;
    while ( !eqish(_d, 0) ) 
    {
        _t = enc.hist.front();
//        _l = dist_at(lidar_config::degree_north);
        _d = DIV( avg_diff , goal );
        
        move(_d);
        synchronize(_d);
#warning Value stored in "_t" is never used
        _t = enc.hist.front();
    }
             
    stop(); // end at stopped position
             
#undef avg_diff
}


bool 
nav::synchronize(double speed) // should go in go forward
{
    using namespace encoder_config;
    using namespace nav_config;
    
    snapshot * state = enc.hist.front();
    
    auto v_L = ABS(get<1>(*state)->_vel);
    auto v_R = ABS(get<2>(*state)->_vel);
    auto v_avg = AVG(v_L , v_R);
    
    auto s_L = left.get_speed();
    auto s_R = right.get_speed();
    auto s_avg = AVG(s_L , s_R);
    
    if(eqish(v_avg,speed)    ? 0x01:0){
        left.set_speed (s_avg);
        right.set_speed(s_avg);
        return true;  
    }
    
    if(v_R < v_avg  ? 0x10:0){ right.set_speed(s_R+max_invariance);    }
    if(v_R > v_avg  ? 0x20:0){ right.set_speed(s_R-max_invariance);    }
    if(v_L < v_avg  ? 0x40:0){ left.set_speed (s_L+max_invariance);    }
    if(v_L > v_avg  ? 0x80:0){ left.set_speed (s_L-max_invariance);    }
    return false;
}


// turn by so many degrees, determined by time
// assuming the front middle is 0 degrees
void 
nav::turn(double angle)
{
    using namespace encoder_config;
    using namespace nav_config;
    
    stop(); // begin from stopped position
    
    //handle edge cases
    if( eqish(angle,0) ){  return;  }
    angle = (angle>360?360:    angle<-360?-360:    angle);
    
    snapshot *_t, *_t0 = enc.hist.front();
    //    int _l , _l0 = dist_at(lidar_config::degree_north);
    
    auto goal = ((angle * double(PI*wheel_base)) * 
                 DIV((PI*wheel_diameter),unit_per_rot));
    
#define _diff( _n_ ) ( get< _n_ >(*_t)->_pos - get< _n_ >(*_t0)->_pos )

    if (angle > 0) 
    {
        left.forward();
        right.backward();
        left.set_speed(std_move_speed);
        right.set_speed(std_move_speed);
    }else{
        right.forward();
        left.backward();
        right.set_speed(std_move_speed);
        left.set_speed(std_move_speed);
    }
    
    double _d1 = 1 , _d2 = 1;
    while ( !eqish(_d1, 0) && !eqish(_d2, 0) ) 
    {
        _t = enc.hist.front();
        //        _l = dist_at(lidar_config::degree_north);
        _d1 = DIV( _diff( 1 ) , goal );
        _d2 = DIV( _diff( 2 ) , goal );
        
        if (angle > 0) 
        {
            left.forward();
            right.backward();
            left.set_speed(std_move_speed);
            right.set_speed(std_move_speed);
        }else{
            right.forward();
            left.backward();
            right.set_speed(std_move_speed);
            left.set_speed(std_move_speed);
        }
        
        synchronize( 0 - AVG(_d1,_d2) );
    }
    
    stop(); // end at stopped position
    
#undef _diff
}

void
nav::move(double speed)
{
    if (speed>0) {
        left.forward();
        right.forward();
    }else{
        left.backward();
        right.backward();
    }
    left.set_speed(speed);
    right.set_speed(speed);
}

void 
nav::veerleft(double ratio)
{
    if(ABS(ratio)==0 ){ return;     }
    if(ABS(ratio)>1  ){ right.set_speed(left.get_speed()*ratio);    }
    if(ABS(ratio)<1  ){ left.set_speed(right.get_speed()*ratio);    }
}


void 
nav::veerright(double ratio)
{
    if(ABS(ratio)==0 ){ return;     }
    if(ABS(ratio)>1  ){ left.set_speed(right.get_speed()*ratio);    }
    if(ABS(ratio)<1  ){ right.set_speed(left.get_speed()*ratio);    }
}


// go forward in a small enough chunk (one cell approx.) so that the distance to the opposing wall can be scanned/updated
void 
nav::goForwardOne()
{
    movedistancevariable(nav_config::cell_size);
}



/* loop the three functions that keep the robot going straight */
void nav::stayStraightLoop()
{
	using namespace nav_config;

	static double lVeer = 1, rVeer = 1;
	int leftORright;
	int counter;

	while (true)
	{
		counter = 0;
		straightAhead(straightLR);
		oneSidedApproach(oneLR);
		twoSidedApproach(twoLR);
		if (straightVal)
			counter++;
		if (oneSideVal)
			counter++;
		if (twoSideVal)
			counter++;

		leftORright = straightLR + oneLR + twoLR; // the decider. will be -1 for left, 0 for nothing, 1 for right
	
		switch (leftORright)
		{
		case -1:{ 
					// nudge left
					rVeer = 1;
					lVeer += max_invariance;
					veerleft(lVeer);
					
			}break;
		case 0: // nothing
			return;
		case 1:{
				   // nudge right
				   lVeer = 1;
				   rVeer += max_invariance;
				   veerright(rVeer);

			}break;

		default:
			break;
		}

		usleep(sleeptime);
	}
}

/* Return true if the function determines that the left center is closer than the right center,
or if the right center is closer than the left center */
void nav::straightAhead(int &leftORright)
{
	using namespace lidar_config;

	// the lidar is called view
	auto scanPoint = view.scan_hist.front();

	// check two packet averages to the left
	auto left1 = scanPoint->at(offset(degree_north, -1));	// packet to the left of dead center
	auto left2 = scanPoint->at(offset(degree_north, -5));	// packet further to the left of dead center
	// get the difference between the left two packet averages
	double differenceLeft = ABS(left1->source->avg_dist - left2->source->avg_dist);

	// check two packet averages to the right
	auto right1 = scanPoint->at(offset(degree_north, 0));	// packet to the right of dead center
	auto right2 = scanPoint->at(offset(degree_north, 4));	// packet further to the right of dead center
	// get the difference between the right two packet averages
	double differenceRight = ABS(right1->source->avg_dist - right2->source->avg_dist);

	double averageLeft, averageRight;

	// if the differences are about the same, then all four packets are on the same wall
	if (nav_config::eqish(differenceLeft, differenceRight))
	{
		// compute the averages of the left two and the right two
		averageLeft = AVG(left1->source->avg_dist, left2->source->avg_dist);
		averageRight = AVG(right1->source->avg_dist, right2->source->avg_dist);

		// compare the averages
		if (averageLeft < averageRight) // the left is closer than the right
		{
			leftORright = -1; // nudge left
			straightVal = true;
		}
		else if (averageRight < averageLeft) // the right is closer than the left
		{
			leftORright = 1; // nudge right
			straightVal = true;
		}
	}
	// the differences aren't about the same, so only use the middle two package averages
	else
	{
		// compare the averages of the middle two
		if (left1->source->avg_dist < right1->source->avg_dist) // the left is closer than the right
		{
			leftORright = -1; // nudge left
			straightVal = true;
		}
		else if (right1->source->avg_dist < left1->source->avg_dist)
		{
			leftORright = 1; // nudge right
			straightVal = true;
		}
	}

	straightVal = false;
}

// return true if the the robot needs to turn
// -1 = turn left. 1 = turn right
void nav::oneSidedApproach(int &leftORright)
{	// assuming 60 degrees north of the eastern heading is the spot where magic happens

	using namespace lidar_config;

	/* get the latest scans, with the second one being grabbed first. 
	   even if there's a scan between, they're guaranteed to be in the correct order*/
	_360_scan * scan3 = view.scan_hist[2];
	_360_scan * scan2 = view.scan_hist[1];
	_360_scan * scan1 = view.scan_hist[0];
	
	/* look at the certain chosen degrees and see if they are readable. 
	   if they are readable, then you must turn left */
	// points along the left wall...
	/* map<uint, const data*>::iterator s1iterator;
	   map<uint, const data*>::iterator s2iterator;*/

	
	auto l60_1 = scan1->at(offset(degree_east, 60));	// latest
	auto l60_2 = scan2->at(offset(degree_east, 60));	// older
	auto l60_3 = scan3->at(offset(degree_east, 60));	// oldest


	if (l60_1->invalid_data)
	{	// if the latest is invalid, but the one before it wasn't, then go right
		if ((!l60_2->invalid_data) || (!l60_3->invalid_data))
		{
			// nudge right
			leftORright = 1;
			oneSideVal = true;
		}
	}
	else if (!l60_2->invalid_data) 
	{
		if (l60_1->distance < l60_2->distance) // if the distance is decreasing
		{
			if (!l60_3->invalid_data)
			{
				if (l60_2->distance < l60_3->distance)
				{
					// nudge right
					leftORright = 1;
					oneSideVal = true;
				}
			}
			// nudge right
			leftORright = 1;
			oneSideVal = true;
		}
	}
	else if(!l60_2->invalid_data) // if the distance is increasing
	{
		if (l60_1->distance > l60_2->distance)
		{
			if (!l60_3->invalid_data)
			{
				if (l60_2->distance > l60_3->distance)
				{
					// nudge right
					leftORright = 1;
					oneSideVal = true;
				}
			}
			// nudge right
			leftORright = 1;
			oneSideVal = true;
		}
	}

	oneSideVal = false;
}

// return true if the robot needs to turn
// -1 = turn left. 1 = turn right
void nav::twoSidedApproach(int &leftORright)
{
	using namespace lidar_config;
	using namespace nav_config;

	// Calculate the averages of the sides up to a certain angle offset for each side.
	// the measurements in a hallway are predictable up to a certain angle, maybe ... degrees north of east/west.
	// use this to ignore the measurements that are too far off, then do the averages

	/* Measure from 60 degrees above the east/west, to 82 degrees above the east/west. 
	This goes up to meet the edge of the degrees that are considered in straightAhead() */


	// Get the latest scan
	_360_scan *vScan = view.scan_hist.front();

	// prepare to average the sides
	double leftCount, rightCount;
	leftCount = 0;	rightCount = 0;

	// iterate through the deque
	double acceptedDistance;
	for (int i = 0; i < 21; i++)
	{
		auto leftIterator = vScan->at(offset(degree_east, 60 + i));	// left side
		auto rightIterator = vScan->at(offset(degree_west, -60 - i));	// right side

		// solve for max accepted distance for this specific angle
		maxAcceptedDistance(60 + i, acceptedDistance);

		if (!leftIterator->invalid_data) // only take valid data
			if (leftIterator->distance <= acceptedDistance)
			{	// only accept distances that are within the projected range for walls parallel to the robot's path
				leftCount += leftIterator->distance;
			}
		if (!rightIterator->invalid_data) // only take valid data
			if (rightIterator->distance <= acceptedDistance)
			{	// only accept distances that are within the projected range for walls parallel to the robot's path
				rightCount += rightIterator->distance;
			}
	}

	// calculate the averages
	double aveLeft, aveRight;
	aveLeft = leftCount / 21;
	aveRight = rightCount / 21;

	// if they aren't similar
	if (!eqish(aveLeft, aveRight))
	{
		if (aveLeft > aveRight)
		{	// if the left side is generally farther away
			// nudge left
			leftORright = -1;
			twoSideVal = true;
		}
		else if (aveRight > aveLeft)
		{
			// nudge right
			leftORright = 1;
			twoSideVal = true;
		}
	}

	twoSideVal = false;
}

// take the absolute value of the angle... it's the same for both sides, so just use 60+i
void nav::maxAcceptedDistance(int angle /*angle B*/, double &acceptedDistance /*side a*/)
{
	double horizontalDistance = 80.0; // 8 centimeters. side c
	double rightAngle = 90.0; // angle A
	double finalAngle = 180 - rightAngle - angle; // angle C

	// solving for side a
	// law of sines . a/sinA = c/sinC
	acceptedDistance = (horizontalDistance * sin(rightAngle)) / sin(finalAngle); // side a
	
	acceptedDistance += 5.0; // account for... it makes more sense this way
}
