//
//  buffer.hpp
//  Car
//
//  Created by Jesse on 26/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef buffer_hpp
#define buffer_hpp

#include <stdio.h>
#include <stdint.h>

#include "global.hpp"

class buffer {
    typedef int positionMarker;

private:
    positionMarker _start, _end;
    const int _length;
    uint8_t * _buffer;

protected:
    positionMarker preIncrement(positionMarker& value);
    positionMarker postIncrement(positionMarker& value);

public:
    buffer(int buffer_size);
    ~buffer();
    
    // getchar() equivalent
    int get();
    
    // putchar(int) equivalent
    int put(int data);
    
    //  items in buffer
    int remaining();
    int available();
    int peek();
};

#endif /* buffer_hpp */
