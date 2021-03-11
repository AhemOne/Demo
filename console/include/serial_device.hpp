//
//  serial_device.hpp
//  Car
//
//  Created by Jesse on 3/11/18.
//  Copyright © 2018 Jesse. All rights reserved.
//
#ifndef ARDUINO_ARCH_AVR
#ifndef serial_device_hpp
#define serial_device_hpp

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "global.hpp"

class serial_device {
private:
    int _fd;
    FILE * _file;
    int _next;
    
protected:
    
    int alive() {
        return _fd && _file;
    }
    
    serial_device(const char * serialFilename) {
        _file = NULL;
        _next = -1;
    }
    
    ~serial_device() {
        end();
        close(_fd);
    }
    
    int available() {
        int data;
        if (_next < 0) if (::read(_fd, &data, 1) > 0) _next = data;
        return (_next < 0)? false : true;
    }

    int read() {
        //int data = fgetc(_file);
        //printf("sd.read = %d\n", data);
        //return data;
        if (available()) {
            int c = _next;
            _next = -1;
            return c;
        } else return -2;
        
        int data;
        return alive()&&fread(&data, 1, 1, _file)?data:-2;
    }
    
    int write(int data) {
        //return fputc(data, _file);
        //if (!alive()) return -1;
        return (int) ::write(_fd, &data, 1);
    }
    
public:
    int begin(speed_t speed) {
        end();
        struct termios terminal;
        
        tcgetattr(_fd, &terminal);
        terminal.c_cflag = CS8 | CREAD;
        cfsetospeed(&terminal, speed);             // set out speed to 38400 baud
        cfsetispeed(&terminal, speed);             // set in speed to 38400 baud
        tcsetattr (_fd, TCSANOW, &terminal);
        
        tcflush(_fd, TCIOFLUSH);
        //cfmakeraw(&_term);
        _file = fdopen(_fd, "r+");              // open as file
        return !!_file;
    }
    
    void end() {
        if (_file) fclose(_file);
        _file = NULL;
    }
};
#endif /* serial_device_hpp */
#endif
