//
//  lidar.cpp
//  micromouse2014
//
//  Created by Lyle Moffitt on 1/21/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#include "lidar.h"
#include <bitset>
#include <fstream>
#include <cmath>



using namespace std;

/***********************      LIDAR METHODS    **************************/

lidar::lidar() : _dev( lidar_config::tty_path )
{ 
    done = false; 
}

lidar::lidar(std::string _path) : _dev( _path )
{ 
    done = false; 
}

uint8_t
lidar::seek()
{
	//cout << "\nSeeking...\n";
    uint8_t _c=0;
	while( 0xFA != _c )
    { 
		_c = _dev.rd_();
		//usleep(10); 
    }
	//cout << "Found!\n";
    return _c;
}

packet*
lidar::scan(uint8_t _seek)
{
    using namespace lidar_config;
    uint8_t _v[pkt_index_max];
	uint8_t * _n =  _dev.sleepy_read(pkt_index_max-1);
    for( int i =1; i<pkt_index_max ; ++i ){ _v[i]=_n[i-1]; }
    return new packet (_v);
}
packet* 
lidar::scan()
{
    return scan(seek());
}

_360_scan*
lidar::build_scan()
{
    _360_scan * _l = new _360_scan;
    while ( _l->add_packet(scan()) < 360);
    scan_hist.push_front(_l);
    if(scan_hist.size() > lidar_config::hist_max){ scan_hist.pop_back(); }
    return  _l;
}

void
lidar::loop()
{
    while( !done ){     build_scan();  }
}


std::thread  
lidar::start_thread()
{ 
    std::cout<<"\nLIDAR started!"<<std::endl;
    return  std::thread( & lidar::loop,&(*this)) ; 
}
