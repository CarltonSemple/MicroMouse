//
//  device.tcc
//  micromouse2014
//
//  Created by Lyle Moffitt on 3/24/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//


/* *******************   Device Attribute Get/Set    ******************** */

using namespace std;

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
    cout<<"[DBG] "<< dev_path<<" << == "<<val <<endl;
    return;
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
    cout<<"[DBG] "<< dev_path<<" == >> "<<val <<endl;
    return val;
}
