//
//  test.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__test__
#define __micromouse2014__test__

#include <iostream>


#include <iostream>
#include <fstream>
#include <string>
#include <strstream>
#include <vector>
#include <set>
#include <map>


/* *****************    Single Path Device      ***************** */
class dev
{
    std::string dev_path;
    
public:
    dev():dev_path("/dev/fd/2"){}
    dev(std::string _n):dev_path(_n){}
    dev(dev & _d):dev_path(_d.dev_path){}
    
    template<class _type>
    void st(_type value);
    
    template<class _type>
    _type gt();
    
};

/********************     Directory Style Device       *********************/
class device_dir
{
    // The path to the root directory of the device file-tree
    std::string dir_path;

    // The device attribute(s) to be controlled.
    std::map<std::string,dev&> dev_attr;
    
public:
	device_dir(std::string _base, 
               std::initializer_list<std::string>  ls_attr);
    
    dev& operator[] (std::string _attr);
    
};


#endif /* defined(__micromouse2014__test__) */
