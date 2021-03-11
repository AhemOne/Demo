//
//  SimpleBuffer.hpp
//  XCIS
//
//  Copyright © 2019 XCIS. All rights reserved.
//

#ifndef SimpleBuffer_hpp
#define SimpleBuffer_hpp

#ifdef ARDUINO
# include <Arduino.h>
#else
# include <iostream>
#endif

#define D_ENABLED
#include "Debug.hpp"

// basic buffer for holding character data
//
// 1. create a buffer
// 1.a. pointed buffer
//          static const size_t buff_length = 64;
//          uint8_t buf_dat[buff_length];
//          simple_buffer<p_buff<buff_length>> buffer(buf_dat);
// 1.b. self buffer
//          static const size_t buff_length = 64;
//          simple_buffer<s_buff<buff_length>> buffer();
// 2. fill buffer
//      buffer can be filled with;
//          putch(char) adding a single character at a time
//          write(char) adding a single character at a time
//          write(read, available) adds from a stream with read() and available()
//      buffer can be read with;
//          copy(char *, size, [from = 0]) copy to buffer from char *, size characters, starting at from. If from is not defined, the copy will start from the first

// template for pointer buffer - uses pointed to memory
template <size_t BUFFER_SIZE>
struct p_buff {
    p_buff() { }
    p_buff(uint8_t * buffer_location) { data = buffer_location; }
    static const size_t max_length = BUFFER_SIZE;
    uint8_t * data;
};

// template for self buffer - creates own memory
template <size_t BUFFER_SIZE>
struct s_buff {
    s_buff() { }
    s_buff(uint8_t * buffer_location) { memcpy(data, buffer_location, max_length); }
    static const size_t max_length = BUFFER_SIZE;
    uint8_t data[BUFFER_SIZE + 1] = {0};
};

// access class for buffers - pass above buffer types as template argument
template <class buffer_t>
class simple_buffer : protected buffer_t {
private:
    size_t position = 0;
    size_t length = 0;
    static const size_t max_length = buffer_t::max_length;
    
public:
    simple_buffer() { }
    simple_buffer(uint8_t * buffer_to_use) : buffer_t(buffer_to_use) { length = strnlen((char *) buffer_to_use, max_length); }
    simple_buffer(const char * buffer_to_use) : buffer_t((uint8_t *) buffer_to_use) { }
    simple_buffer * me() { return this; }
    
    int putch(uint8_t a) {
        //dprint("\t\tputting %c in position %u\n", a, position);
        if (position == length) {
            if (length < max_length) length++;
            else return -1;
        }
        buffer_t::data[position++] = a;
        buffer_t::data[position] = 0;
        return a;
    }
    
    // stream functionality
    int available() { return (int) (length - position); }
    
    void rewind() { position = 0; }
    void flush() { rewind(); buffer_t::data[0] = 0x00; length = 0; }
    
    void setpos(const size_t i) {
        if (i < length) position = i;
    }
    
    // stream reading
    
    uint8_t * data() {
        //dprint("\t\tcollecting data: %s\n", buffer_t::data);
        return buffer_t::data;
    }
    
    char * string() {
        //dprint("\t\tcollecting string: %s\n", buffer_t::data);
        return (char *) buffer_t::data;
    }
    
    int copy(uint8_t * to, size_t size, int from = 0) {
        int number_copied = 0;
        while (from < size && from < max_length) {
            to[number_copied] = buffer_t::data[from];
            from++;
            number_copied++;
        }
        //dprint("copied %d chars %s\n", number_copied, to);
        return number_copied;
    }
    
    int peek() { return buffer_t::data[position]; }
    
    // stream writing
    size_t write(const uint8_t & c) { return putch(c) >= 0; }
    size_t write(const uint8_t *buffer, const size_t & size) {
        size_t written = 0;
        //dprint("writing %u chars: %s\n", size, buffer);
        for (size_t i = 0; i < size; i++) written += write(buffer[i]);
        return written;
    }
    size_t write(const char string[]) {
        size_t N = strlen(string);
        size_t n, i = 0;
        for ( n = 0; n < N; n++ ) i += write(string[n]);
        return i;
    }
    
    size_t size() { return length; }
    
    size_t counttokens(const uint8_t & separator) {
        if ( length == 0 ) return 0;
        
        size_t total_tokens = 1;
        for (size_t i = 1; i < length; i++) if ( buffer_t::data[i] == separator ) {
            //dprint("separator seen at %u [%c]\n", i, (char) buffer_t::data[i]);
            total_tokens++;
        }
        return total_tokens;
    }
    
    size_t parsetokens(uint8_t * tokens[], const uint8_t & token) {
        if ( !counttokens(token) ) return 0;
        
        char terminator = 0x00;
        uint8_t * p = buffer_t::data;
        
        size_t token_count = replace(token, terminator);
        size_t token_n = 1;
        tokens[0] = p;
        //dprint("token added %s\n", tokens[0]);
        for (size_t i = 1; i < length && token_n < token_count; i++) {
            if ( p[i] == terminator ) {
                tokens[token_n] = &p[i + 1];
                //dprint("token added %s\n", tokens[token_n]);
                token_n++;
            }
        }
        return token_n;
    }
    
    size_t replace(const uint8_t & from, const uint8_t & to) {
        if ( length == 0 ) return 0;
        
        uint8_t * const p = buffer_t::data;
        
        // count the tokens
        for (size_t i = 0; i < length; i++) if ( p[i] == from ) {
            p[i] = to;
            //dprint("character changed at %u\n", i);
        }
        return counttokens(to);
    }
    
    operator char * () {
        return string();
    }
    
    operator uint8_t * () {
        return data();
    }
};

template <size_t size> using local_simple_buffer = simple_buffer<s_buff<size>>;
template <size_t size> using pointed_simple_buffer = simple_buffer<p_buff<size>>;

#endif /* SimpleBuffer_hpp */
