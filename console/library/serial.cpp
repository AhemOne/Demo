//
//  serial.cpp
//  Car Mac
//
//  Created by Jesse on 8/8/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef ARDUINO_ARCH_AVR
#include "serial.hpp"

/*
serial::serial() {
    _fd = -1;
}

serial::serial(const char * serialFilename) {
    _buffer = new buffer(SERIAL_BUFFER_SIZE);
    _fd = open(serialFilename, O_NDELAY | O_RDWR);
    _file = NULL;
}

serial::~serial() {
    end();
    delete _buffer;
    close(_fd);
}

int serial::begin(speed_t speed) {
    end();
    struct termios terminal;
    for (int i = 0; i < sizeof(termios); i++) ((char *)&terminal)[i] = 0;
    
    tcgetattr(_fd, &terminal);
    terminal.c_cflag = CS8 | CREAD;

    cfsetospeed(&terminal, speed);             // set out speed to 38400 baud
    cfsetispeed(&terminal, speed);             // set in speed to 38400 baud
    tcsetattr (_fd, TCSANOW, &terminal);
    
    //cfmakeraw(&_term);
    _file = fdopen(_fd, "r+");              // open as file
    return !!_file;
}

void serial::update() {
    if (_file) {
        int data;
        while (_buffer->available() < SERIAL_BUFFER_SIZE) {
            data = fgetc(_file);
            if (data < 0) break;
            _buffer->put(data);
 
            //::read(_fd,&data,1);
            //if (data <= 0) break;
            //else {
            //    if ((char)data == 1 || (char)data == '\n') break;
            //}
 
            
            //printf("pushing into _buffer: %c (%.2x) -> %c (%.2x)\n", iscntrl(data)?0:data, data, iscntrl(putreturn)?0:data, putreturn);

        }
            
            //&& (data = fgetc(_file) != -1)) _buffer->put(data);
    }
}

void serial::end() {
    if (_file) fclose(_file);
    _file = NULL;
}

int serial::available() {
    update();
    return _buffer->available();
}

int serial::read() {
    int data = _buffer->get();
    //printf("pulling %c (%.2x) from _buffer\n", iscntrl(data)?0:data, data);
    return data;
}

int serial::peek() {
    return _buffer->peek();
}

int serial::putchar(int data) {
    return fputc(data, _file);
}
*/
#endif
