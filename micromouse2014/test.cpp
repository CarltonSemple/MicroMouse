//
//  test.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "test.h"

using namespace std;

/********************     Directory Style Device       *********************/
device_dir::device_dir(string _base, 
                       initializer_list<string>  ls_attr):
dir_path(*new string(_base))
{
    for( auto it = ls_attr.begin(); it != ls_attr.end(); it++)
    {
        dev_attr.emplace(pair<string,dev&>( string(*it),
                                            *new dev(string(_base + *it)) ));
    }
}


dev&
device_dir::operator[](string _attr)
{
    return dev_attr.at(_attr);
}

/* *******************   Device Attribute Get/Set    ******************** */
template <class _type>
void
dev::st(_type val)
{
    ofstream _f ;
    _f.open(dev_path.c_str());
    if(_f.bad() || !_f.good())
    {   
        cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); return ;
    }
    _f << val;
    _f.close();
}

template <class _type>
_type
dev::gt()
{
    ifstream _f;
    _f.open(dev_path.c_str());
    if(_f.bad() || !_f.good())
    {   
        cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); throw exception();
    }
    _type val;
    _f >> val;
    _f.close();
    return val;
}
