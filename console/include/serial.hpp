//
//  serial.hpp
//  Car Mac
//
//  Created by Jesse on 8/8/18.
//  Copyright © 2018 Jesse. All rights reserved.
//
#ifndef ARDUINO_ARCH_AVR
#ifndef serial_hpp
#define serial_hpp

//#include <stdlib.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <unistd.h>
//#include <termios.h>
//#include <fcntl.h>

//#include "global.hpp"
//#include "stream.hpp"
//#include "buffer.hpp"

#include "stream.hpp"
#include "serial_device.hpp"
#include "buffer.hpp"

class serial : public serial_device, public Stream {
    buffer _buffer;
    
public:
    serial(char * serial_location) : serial_device(serial_location), _buffer(SERIAL_BUFFER_SIZE) { }
    serial(const char * serial_location) : serial_device((char *)serial_location), _buffer(SERIAL_BUFFER_SIZE) { }
    
    int available() {
        enable_debug(serial::available());
        while (_buffer.remaining() && serial_device::available()) {
            int c = serial_device::read();
            debug("s.read %d\n", c);
            if (c <= 0) break;
            else _buffer.put(c);
        }
        return _buffer.available();
    }
    int read() {return available()?_buffer.get():-4;}
    int peek() {return available()?_buffer.peek():-5;}
    int putchar(int data) {return serial_device::alive()?serial_device::write((char)data):-3;}
};

/*
class serial : public Stream {
private:
    int _fd;
    FILE * _file;
    
    buffer * _buffer;
    
protected:
    void update();
        
public:
    serial();
    serial(const char * serialFilename);
    ~serial();

    int begin(speed_t speed);
    void end();
    
    int available();
    int read();
    int peek();
    int putchar(int data);
};
*/
#endif /* serial_hpp */
#endif
