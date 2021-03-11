//
//  stream.cpp
//  Car
//
//  Created by Jesse on 25/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef ARDUINO_ARCH_AVR
#include "stream.hpp"

int Stream::LF() {
    return '\n';
}

int Stream::CR() {
    return '\r';
}

int Stream::newline() {
    putchar('\n');
    return 2;
}

int Stream::write(uint8_t val) {
    return val && putchar(val);
}

int Stream::write(char str[]) {
    int i = 0;
    while (str[i] != 0) i += write(str[i]);
    return i;
}

int Stream::write(char * buf, int len) {
    for (int i = 0; i < len; i++) {
        write(buf[i]);
    }
    return len;
}

int Stream::print(int val) {
    char buf[32];
    int len = sprintf(buf, "%d", val);
    return write(buf, len);
}

int Stream::print(float val) {
    char buf[32];
    int len = sprintf(buf, "%f", val);
    return write(buf, len);
}

int Stream::print(char val) {
    return write(val);
}

int Stream::print(char val[]) {
    return write(val);
}

int Stream::print(int val, int mode) {
    char buf[32];
    int len = 0;
    switch (mode) {
        case BIN:
        {
            for (int i = val; i; i /= 2) len++;
            int pos = len;
            do {
                buf[pos--] = val % 2;
                val /= 2;
            } while (pos);
            break;
        }
        case OCT:
            len = sprintf(buf, "%o", val);
            break;
            
        case DEC:
            len = sprintf(buf, "%d", val);
            
            break;
            
        case HEX:
            len = sprintf(buf, "%X", val);
            break;
            
        default:
            break;
    }
    return write(buf, len);
}

int Stream::print(float val, int points) {
    char buf[32];
    char expr[8];
    sprintf(expr, "%c.%df", '%', points); // "%.<#>f"
    int len = sprintf(buf, expr, val);
    return len;
}

int Stream::print(char val[], int len) {
    int number = 0;
    for (int i = 0; i < len; i++) {
        if (val[i] == 0) break;
        else {
            write(val[i]);
            number++;
        }
    }
    return number;
}

int Stream::println(int val) {
    int len = print(val);
    len += newline();
    return len;
}
int Stream::println(float val) {
    int len = print(val);
    len += newline();
    return len;
}
int Stream::println(char val) {
    int len = print(val);
    len += newline();
    return len;
}

int Stream::println(char val[]) {
    int len = print(val);
    len += newline();
    return len;
}

int Stream::println(int val, int mode) {
    int len = print(val, mode);
    len += newline();
    return len;
}

int Stream::println(float val, int points) {
    int len = print(val, points);
    len += newline();
    return len;
}

int Stream::println(char val[], int to) {
    int len = print(val, to);
    len += newline();
    return len;
}

int Stream::print(const char str[]) {
    return print((char *)str);
}

int Stream::println(const char str[]) {
    int len = print(str);
    len += newline();
    return len;
}

#endif
