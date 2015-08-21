//
//  formatting.h
//  micromouse2015
//
//  Created by Lyle Moffitt on 9/25/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef __micromouse2014__formatting__
#define __micromouse2014__formatting__


#include <cassert>
#include <stdexcept>

/** Asserts or throws if NDEBUG is defined
 \def assert_or_throw
 */
#ifdef NDEBUG
#define assert_or_throw( _test_ , _except_ , _what_ )\
if( _test_ ) throw _except_( _what_ "\n\t[[" #_test_ "]]" __FILE__":"__LINE__ )
#else
#define assert_or_throw( _test_ , _except_ , _what_ )\
assert( _test_ && _what_ )
#endif

#include <string>
/** Creates a formatted, printf-style string
 \param     format      The formatting string \see printf()
 \param     f_args      The arguments for the format string
 \return    A \c std::string constructed in the style of \c printf()
 \see http://en.cppreference.com/w/cpp/io/c/fprintf
 */
template<class ... var_t> 
inline 
std::string
stringf( const char * format, var_t ... f_args )
{
    using namespace std;
    size_t _sz = (sizeof...(f_args)) * sizeof(size_t) * string(format).size();
    char buf [_sz];
    memset(&buf, 0, _sz);
    std::sprintf(buf, format, f_args... );
    return std::string( buf );
}


/** Returns a lowercase copy of the input string
 */
std::string to_lower(std::string & _s);


#include <cassert>
#include <array>
template<typename _type>
struct init_ls : public std::initializer_list<_type>
{
    init_ls( std::initializer_list<_type> _ls):     
    std::initializer_list<_type>(std::move(_ls))
    {
    }

    
    const _type&
    operator[](size_t _sz)
    {   assert_or_throw(begin(*this)!=end(*this), std::out_of_range,
                        "Access operator not valid with empty init list.");
        assert_or_throw(_sz >= this->size(), std::invalid_argument,
                        "Element access out of bounds.");
        return *(this->begin()+_sz);
    }
};


#endif //__micromouse2014__formatting__
