//
//  device.cpp
//  Car
//
//  Created by Jesse on 20/11/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "device.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>

#include "packet.hpp"

class device {
private:
    std::queue<packet::packet_t *> _input;
    std::queue<packet::packet_t *> _output;
    
protected:
    void put(packet::packet_t * output) {
        _output.push(output);
    }
    
    packet::packet_t * get() {
        packet::packet_t * input = _input.front();
        _input.pop();
        return input;
    }
    
    size_t incoming_available() {
        return _input.size();
    }
    
    size_t outgoing_available() {
        return _output.size();
    }
    
public:
    virtual int start(void * input) {
        return EXIT_SUCCESS;
    }
    
    virtual int update(void * input) {
        printf("hello\n");
        usleep(1000000);
        return EXIT_SUCCESS;
    }
    
    virtual int end(void * input) {
        return EXIT_SUCCESS;
    }
    
    void write(packet::packet_t * input) {
        _input.push(input);
    }
    
    packet::packet_t * read() {
        packet::packet_t * output = _output.front();
        _output.pop();
        return output;
    }
    
    size_t available() {
        return outgoing_available();
    }
};
