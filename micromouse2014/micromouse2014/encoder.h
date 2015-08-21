//
//  encoder.h
//  Test
//
//  Created by Lyle Moffitt on 3/19/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef Test_encoder_h
#define Test_encoder_h

#include "tty.h"

#include <deque>
#include <utility>
#include <thread>

class encoder;
struct measure;


namespace encoder_config
{    
    static auto path_ls   =
    {
        "/dev/ttyO2"
    };

    typedef std::chrono::time_point<std::chrono::system_clock>::rep timestamp;

    typedef int32_t position_t;
    typedef float   velocity_t;
    typedef float   xlr8tion_t;
    
    constexpr size_t hist_max = 100;
    
    constexpr size_t msg_size = ( (sizeof(position_t)+sizeof(uint8_t))
                                 +(sizeof(velocity_t)+sizeof(uint8_t))
                                 +(sizeof(xlr8tion_t)+sizeof(uint8_t))
                                 + sizeof(uint8_t)*2
                                 );
    
    constexpr size_t msg_freq_microsec =  250;

    constexpr uint8_t EOE/*End-Of-Encoding*/ = '\0';
    
    typedef std::tuple< timestamp, measure*, measure* > snapshot ;
};

struct measure
{
    // Which motor this is referencing; 'L'eft -or- 'R'ight
    char side;
    
    // Relatave position of wheel in mm; 
    encoder_config::position_t    _pos;
    
    // Average velocity of wheel in mm/sec (based on last two positions)
    encoder_config::velocity_t    _vel;
    
    // Average acceleration in mm/sec^2 (based on last two velocities)
    encoder_config::xlr8tion_t    _accl;
};

class encoder : device_tty
{
    /// Add provided measurements to the history deque
    inline 
    encoder_config::snapshot* 
        add_hist(measure* _L, measure* _R);
    
    /// Temporart data storage; holds last N snapshots
    std::deque<encoder_config::snapshot*> temp_hist;
    
protected:
    bool done;
    
     std::mutex safety;
    
public:
    
    /// Primary data storage; holds last N snapshots
    std::deque<encoder_config::snapshot*> hist;
    
    /// Defualt constructor; relies on config path
    encoder(): device_tty( encoder_config::path_ls.begin()[0])
    {done = false;}
    
    /// Explicit constructor; requires provision of dev path  
    encoder( const char* _path ):device_tty(_path)
    {}
 
    /// Updates the history with most recent measurements from wheels
    encoder_config::snapshot* 
        update();
    
    /// Infinite loop of updates
    void loop();
    
    std::thread start_thread()
    { 
        std::cout<<"\nEncoder started!"<<std::endl;
        return std::thread(&encoder::loop,&(*this)); 
    }
    
    /// Get a safe iterator for sequential reads. STOPS
    std::deque<encoder_config::snapshot*>::iterator
        get_hist_iterator();
    
    /// Update scan_hist with temp data
    void done_scan_iterator();
    
};


#endif
