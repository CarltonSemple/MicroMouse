//
//  formatting.cpp
//  micromouse2015
//
//  Created by Lyle Moffitt on 39/25/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "formatting.h"


#include <string>

std::string to_lower(std::string &_s)
{
    std::string _d;
    for (auto i : _s ) { _d += std::tolower(i) ;}
    return _d;
}

