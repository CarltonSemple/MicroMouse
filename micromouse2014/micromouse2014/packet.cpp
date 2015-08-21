//
//  packet.cpp
//  micromouse
//
//  Created by Lyle Moffitt on 3/7/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//


#include "packet.h"
#include <iostream>
#include "macros.h"
using namespace  packet_config;

/* ********************             DATA            ******************** */
data::data(const packet * _p, const uint8_t _d[data_len]):
distance        (_d[0] | (_d[1]&0x3F)<<8 ),
strength_warn   (_d[1] & 0x40),
invalid_data    (_d[1] & 0x80),
strength        (_d[2] | _d[3]<<8 ),
source          (_p)
{}

void 
data::print(uint index) const
{
    using namespace std;
    cout << "\nData "<<index<<":";
    cout << "\n     Dist. : ";
    if (!invalid_data) cout << distance;
    else printf("INVALID : %04X",distance);
    cout << "\n     Strn. : ";
    if (!strength_warn ) cout << strength;
    else cout << "INVALID";
    return ;
}


/* ******************               PACKET            ****************** */



packet::packet(const uint8_t _v[packet_len]):
p_time(std::chrono::system_clock::now().time_since_epoch().count()),
start(_v[0]),
index(_v[1]),
speed([](int16_t a)
{   int16_t b=0;
    for (uint16_t _u = 0x0001; _u&0xFFFF; _u<<=1){ b= (b<<1) + (_u & a); }
    return b;
}( _v[2]<<8 | _v[3] )),
d0(this, &_v[ 4]),
d1(this, &_v[ 8]),
d2(this, &_v[12]),
d3(this, &_v[16]),
chksum(_v[21]<<8 | _v[20]),
chkcalc([&_v](){
    uint16_t chk_tab[10]; int32_t chk32=0;
    for (int i =0; i< 10 ; ++i){ chk_tab[i]=( _v[2*i] | _v[2*i+1]<<8 ); }
    for (auto &i: chk_tab){ chk32 = (chk32<<1) + i; }
    return ( 0x7FFF & ((chk32 & 0x00007FFF) + (chk32 >> 15)) );
    
}()),
avg_dist([&](){
    int num=0,sum=0;
    if(!d0.invalid_data){ sum+=d0.distance; num++; }
    if(!d1.invalid_data){ sum+=d1.distance; num++; }
    if(!d2.invalid_data){ sum+=d2.distance; num++; }
    if(!d3.invalid_data){ sum+=d3.distance; num++; }
    return double (DIV(sum, num));
}())
{
}


void 
packet::print() const
{
    using namespace std;
    auto val = p_time;//.time_since_epoch().count();
    printf("\nConstents of packet #%016llX",val);
    printf("\nStart byte : %02X", start);
    printf("\nIndex byte : %02X", index);
    printf("\nSpeed value: %04X", speed);
    d0.print(0);
    d1.print(1);
    d2.print(2);
    d3.print(3);
    cout << "\nChecksum: ";
    if (chkcalc==chksum) cout<<"MATCH";
    else printf("ERR %04X != %04X",chkcalc,chksum);
    cout<<endl;
    return;
}


/***********************      SCAN_MAP METHODS    ***********************/

size_t
_360_scan::add_packet(packet* _p)
{
//    using namespace packet_config;
    
    if (_p->index == index_min) { t_begin = _p->p_time; }
    if (_p->index == index_max) { t_end = _p->p_time; }

    uint degree = (_p->index - index_min) * data_num;
    //cout <<"\nAdding degree: "<< degree;
    (*this)[degree + 0]= &_p->d0 ;
    (*this)[degree + 1]= &_p->d1 ;
    (*this)[degree + 2]= &_p->d2 ;
    (*this)[degree + 3]= &_p->d3 ;
//    emplace( pair<uint,data>(degree + 0, _p.d0) ) ;
//    emplace( pair<uint,data>(degree + 1, _p.d1) ) ;
//    emplace( pair<uint,data>(degree + 2, _p.d2) ) ;
//    emplace( pair<uint,data>(degree + 3, _p.d3) ) ;
    //cout<< "\nAdding packet #" <<size();
    //_p.print();
    return (*this).size();
}



void
_360_scan::print_pkt()
{
	//cout << "\nPrinting...";
    for(auto it : (*this) )
    {
        if( (it.first%4 ==0)  )
            it.second->source->print();

    }
}

void
_360_scan::print_data()
{
	//cout << "\nPrinting...";
    for(auto it : (*this) )
    {
        it.second->print(it.first%4);
    }
}
