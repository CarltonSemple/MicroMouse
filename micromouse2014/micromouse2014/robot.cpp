//
//  robot.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/20/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "robot.h"

using namespace std;

robot::robot():
navigator(view, enc),
algo(view,navigator)
{
    thread t_l ( move( view.start_thread()  )   ); 
    t_l.join();
    
    thread t_e ( move( enc.start_thread()   )   ); 
    t_e.join();
    
    sleep(3);
    
//    thread t_n ( & navigator.start_thread() ); 
//    t_n.join();
    
    thread t_s ( move( algo.start_thread()  )   ); 
    t_s.join();
    
}


