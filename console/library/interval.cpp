//
//  interval.cpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "interval.hpp"

#ifndef ARDUINO_ARCH_AVR
#include "../include/interval.hpp"

void intervalTimer::setNextTime() {
    _next.tv_usec += _waitPeriod;
    if (_next.tv_usec >= 1000000) {
        _next.tv_sec++;
        _next.tv_usec -= 1000000;
    }
}

long int intervalTimer::getTimeDifference() {
    timeval now;
    gettimeofday(&now, NULL);
    
    now.tv_usec = _next.tv_usec - now.tv_usec;
    now.tv_sec = _next.tv_sec - now.tv_sec;
    if (now.tv_usec < 0) {
        now.tv_sec--;
        now.tv_usec += 1000000;
    }
    return now.tv_sec * 1000000 + now.tv_usec;
    
}


intervalTimer::intervalTimer(unsigned int waitTime_us) : _waitPeriod(waitTime_us) {
    gettimeofday(&_next, NULL);
    reset();
}

bool intervalTimer::wait() {
    while (!timeUp()) {
        usleep(_waitPeriod / 4);
    }
    reset();
    return 1;
}

bool intervalTimer::timeUp() {
    return getTimeDifference() > 0 ? false : true;
}

void intervalTimer::reset() {
    setNextTime();
}

intervalCounter::intervalCounter(unsigned int count_ms) : _waitCount(count_ms) {
    _countdown = 0;
}

bool intervalCounter::ready(bool doReset) {
    bool isReady = false;
    if (_countdown > 0) {
        _countdown--;
    } else {
        isReady = true;
        if (doReset) reset();
    }
    //printf("Interval: %d ready: %d\n", _countdown, isReady);
    return isReady;
}

void intervalCounter::reset() {
    _countdown = _waitCount - 1;
}

frameTimer::frameTimer(unsigned int FPS) : intervalTimer(1000000/FPS) {
}

bool frameTimer::ready() {
    if (timeUp()) {
        reset();
        return 1;
    } else return 0;
}

#endif
