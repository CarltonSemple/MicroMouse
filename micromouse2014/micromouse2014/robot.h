//
//  robot.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__robot__
#define __micromouse2014__robot__

#include <iostream>
#include <thread>


//#include "lidar.h"
#include "star.h"
//#include "nav.h"

namespace robot_config 
{
    // Maximum width of the robot
    constexpr size_t max_width = 250;//mm
    
    // Maximum length of the robot
    constexpr size_t max_length = 250;//mm
}


struct robot
{
    lidar   view;
    encoder enc;
    nav     navigator;
    star    algo;
    
    
    robot();    
};


#endif /* defined(__micromouse2014__robot__) */
