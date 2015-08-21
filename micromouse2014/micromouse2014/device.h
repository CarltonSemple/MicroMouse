//
//  device.h
//  Test
//
//  Created by Lyle Moffitt on 3/6/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

//#ifndef Test_device_h
#define Test_device_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>

//#include <typeinfo>
//#include <typeindex>





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
    device_dir(const char* _base, 
               std::initializer_list<const char*>  ls_attr);
    dev& operator[] (std::string _attr);
    dev& operator[] (const char* _attr);
    
};



#include "device.tcc"



//#endif
