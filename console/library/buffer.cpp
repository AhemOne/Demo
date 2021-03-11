//
//  buffer.cpp
//  Car
//
//  Created by Jesse on 26/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "buffer.hpp"

buffer::positionMarker buffer::preIncrement(positionMarker& value) {
    ++value;
    if (value == _length) value = 0;
    return value;
}

buffer::positionMarker buffer::postIncrement(positionMarker& value) {
    int old = value;
    preIncrement(value);
    return old;
}

buffer::buffer(int buffer_size) : _length(buffer_size+1) {
    _buffer = new uint8_t[_length];
    _start = _end = 0;
}

buffer::~buffer() {
    delete [] _buffer;
}

// getchar() equivalent
int buffer::get() {
    return available()?_buffer[postIncrement(_start)]:-1;
    
    if (_start == _end) return -1; // no data, return nothing
    else {
        return _buffer[postIncrement(_start)];
    }
}

// putchar(int) equivalent
int buffer::put(int data) {
    return remaining()?(_buffer[postIncrement(_end)] = data & 0x00ff):-1;
    //enable_debug(buffer::put);
    int _last = _end;
    if (preIncrement(_last) == _start) return -1;
    else return _buffer[postIncrement(_end)] = data & 0x00ff;
}

//  items in buffer
int buffer::available() {
    int _back = _end;
    if ( _back < _start ) _back += _length;
    return _back - _start;
}

int buffer::peek() {
    if (available()) return _buffer[_start];
    else return -1;
}

int buffer::remaining() {
    return _length - available() - 1;
}
