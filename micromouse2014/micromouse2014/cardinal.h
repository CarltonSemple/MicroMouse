//
//  cardinal.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 10/13/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef micromouse2014_cardinal_h
#define micromouse2014_cardinal_h

#include <tuple>
#include <array>
#include <utility>
#include <vector>
#include <functional>

enum DIR 
{
	_N_=0,
	_S_=1,
	_E_=2,
	_W_=3
};


template <class _type>
struct rose 
{
//    typedef typename std::conditional<std::is_reference<_type>::value,std::reference_wrapper<_type>,_type>::type val_type;
    typedef std::reference_wrapper<_type> ref_type;
    std::tuple<_type,_type,_type,_type> _tup;
//    _type   north, south, east, west;
    
    _type&      N(){    return std::get<DIR::_N_>(_tup);  }
    _type&      E(){    return std::get<DIR::_E_>(_tup);   }
    _type&      S(){    return std::get<DIR::_S_>(_tup);  }
    _type&      W(){    return std::get<DIR::_W_>(_tup);   }
    
    /// Get using enum direction
    _type& operator[](DIR d)
    {   
        switch(d){
            case DIR::_N_: return  std::get<DIR::_N_>(_tup); 
            case DIR::_E_: return  std::get<DIR::_E_>(_tup); 
            case DIR::_S_: return  std::get<DIR::_S_>(_tup);
            case DIR::_W_: return  std::get<DIR::_W_>(_tup);
        } 
    }
    
    /// Get using char for direction
    _type& operator[](const char d)
    { 
        switch(d){
            case 'N':case 'n': return  std::get<DIR::_N_>(_tup); 
            case 'E':case 'e': return  std::get<DIR::_E_>(_tup); 
            case 'S':case 's': return  std::get<DIR::_S_>(_tup);
            case 'W':case 'w': return  std::get<DIR::_W_>(_tup);
            default: throw std::invalid_argument
                ("Argument must be one of {N,E,S,W,} or {n,e,s,w}");
        }
    }
    

    /// Default constructor
    
    rose():_tup()
    {
        std::vector<int> v;
    }
    
    
    /// Copy constructor
    rose(const _type &_n, const _type &_e, const _type &_s, const _type &_w):
        _tup( _n, _e, _s, _w )
    {}
    
    /// Copy constructor
    explicit 
    rose(std::array<_type,4> _r):
        _tup( _r[DIR::_N_],_r[DIR::_E_],_r[DIR::_S_],_r[DIR::_W_] )
    {}
    
    /// Copy constructor
    rose(rose &_r):
        _tup( _r[DIR::_N_],_r[DIR::_E_],_r[DIR::_S_],_r[DIR::_W_] )
    {}
    

    /// Assignment operator
    rose& operator=(std::tuple<_type,_type,_type,_type> _r)
    {
        std::get<DIR::_N_>(_tup) = std::get<DIR::_N_>(_r);
        std::get<DIR::_E_>(_tup) = std::get<DIR::_E_>(_r);
        std::get<DIR::_S_>(_tup) = std::get<DIR::_S_>(_r);
        std::get<DIR::_W_>(_tup) = std::get<DIR::_W_>(_r);
        return *this;
    }
    
    /// Assignment operator
    rose& operator=(const rose& _r)
    {
        std::get<DIR::_N_>(_tup) = std::get<DIR::_N_>(_r._tup);
        std::get<DIR::_E_>(_tup) = std::get<DIR::_E_>(_r._tup);
        std::get<DIR::_S_>(_tup) = std::get<DIR::_S_>(_r._tup);
        std::get<DIR::_W_>(_tup) = std::get<DIR::_W_>(_r._tup);
        return *this;
    }
    
    
};

typedef std::integral_constant<char, 'N'> north_char;
typedef std::integral_constant<char, 'S'> south_char;
typedef std::integral_constant<char, 'E'> east_char;
typedef std::integral_constant<char, 'W'> west_char;

typedef std::integral_constant<int, 0> north_int;
typedef std::integral_constant<int, 1> south_int;
typedef std::integral_constant<int, 2> east_int;
typedef std::integral_constant<int, 3> west_int;

typedef std::integral_constant<DIR, DIR::_N_> north_dir;
typedef std::integral_constant<DIR, DIR::_S_> south_dir;
typedef std::integral_constant<DIR, DIR::_E_> east_dir;
typedef std::integral_constant<DIR, DIR::_W_> west_dir;


#endif





