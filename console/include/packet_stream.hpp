//
//  serial_manager.hpp
//  Car
//
//  Created by Jesse on 1/11/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef packet_stream_hpp
#define packet_stream_hpp

#include <stdio.h>

#include "stream.hpp"
#include "packet.hpp"

class packet_stream {
private:
    Stream * _stream;
    packet::packet_t _buffer;
    int _buffer_position;
    
public:
    packet_stream(Stream * stream) {
        _stream = stream;
        _buffer[0] = '\0';
        _buffer_position = 0;
    }
    
    int packet_is_waiting() {
        if (_buffer_position && _buffer[_buffer_position - 1] == '\n') {
            if (packet::is_valid(&_buffer)) {
                printf("packet valid\n");
                return true;
            } else {
                printf("packet not valid\n");
                _buffer_position = 0;
            }
        } else {
            if (_buffer_position == sizeof(_buffer)) {
                printf("packet read overflow\n");
                _buffer_position = 0;
            }
        }
        if (_stream->available()) {
            int c = _stream->read();
            printf("c = %c\n", c);

            if (c == '+' || (_buffer_position && _buffer_position < _buffer.length())) {
                if (c == '+' && _buffer_position) _buffer_position = 0;
                _buffer[_buffer_position++] = (char) c & 0xff;
                printf("_buff[%d] = %c\n", _buffer_position - 1, c);
                return packet_is_waiting();
            } else {
                printf("bad read in piw: %c\n", c);            }
        }
        //printf("nothing to report\n");
        return 0;
    }
    
    int receive(packet::packet_t * to) {
        if (packet_is_waiting()) {
            printf("something is available %d -> %d\n", _buffer_position, _stream->available());
            _buffer_position = 0;
            
            strncpy((*to)(), _buffer(), sizeof(_buffer));
            for (char * i = strchr((*to)(), '\n'); i < &(*to)[sizeof(_buffer)]; i++) *i = '\0';
            return (int) strlen((*to)());
        } else {
            return 0;
        }
    }
    
    int send(packet::packet_t * from) {
        return _stream->write((*from)());
    }
};

#endif /* packet_stream_hpp */
