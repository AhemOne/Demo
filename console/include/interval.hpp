//
//  interval.hpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef interval_hpp
#define interval_hpp

#include <stdio.h>
#include <sys/time.h>   // for timeval and gettimeofday()
#include <unistd.h>     // for usleep()

class intervalTimer {
private:
    const unsigned int _waitPeriod;
    timeval _next;
    
protected:
    void setNextTime();
    
public:
    long int getTimeDifference();
    
public:
    intervalTimer(unsigned int waitTime_us);
    
    bool wait();        // blocking until time is up
    
    bool timeUp();      // seeks if time is up, non blocking
    void reset();       // adds a new time interval
};

class intervalCounter {
private:
    const unsigned int _waitCount;
    unsigned int _countdown;
    
protected:
    
public:
    intervalCounter(unsigned int count_ms);
    
    bool ready(bool doReset = true);      // seeks if time is up, non blocking
    void reset();       // resets timer
};

class frameTimer : public intervalTimer {
    
public:
    frameTimer(unsigned int FPS = 25);
    
    bool ready();
};

#endif /* interval_hpp */
