//
//  stream.hpp
//  Car
//
//  Created by Jesse on 25/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef stream_hpp
#define stream_hpp

#include <stdio.h>
#include <stdint.h>

#define BIN 0x1000
#define OCT 0x2000
#define DEC 0x4000
#define HEX 0x8000

class Stream {
private:
    int LF();
    int CR();
protected:
    int newline();
public:    
    virtual int available() {return 0;}
    virtual int read() {return -6;}
    virtual int peek() {return -7;}
    
    virtual int putchar(int data) {return -8;}
    
    int write(uint8_t val);
    int write(char str[]);
    int write(char * buf, int len);
    
    int print(int);
    int print(float);
    int print(char);
    int print(char[]);
    int print(const char str[]);
    int print(int, int);
    int print(float, int);
    int print(char[], int);
    
    int println(int);
    int println(float);
    int println(char);
    int println(char[]);
    int println(const char str[]);
    int println(int, int);
    int println(float, int);
    int println(char[], int);
};

class packetstream {
    
};

#endif /* stream_hpp */
