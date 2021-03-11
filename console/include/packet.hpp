//
//  packet.hpp
//  Car
//
//  Created by Jesse on 30/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef packet_hpp
#define packet_hpp

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "global.hpp"

namespace packet {
    template <int size>
    class packet_type {
    private:
        char data[size+1];
        
    public:
        const int length() { return size; }
        
        ////// () operators //////
        packet_type() { data[0] = '\0'; }
        packet_type(const char * input) { strncpy(data, input, size+1); }
        packet_type(char * input) { strncpy(data, input, size+1); }
        char * operator()() { return data; }
        packet_type& operator()(const packet_type & input) { return operator()(input.data); }
        packet_type& operator()(const char * input) {
            char expr[16];
            sprintf(expr, "%%.%ds", size);
            sprintf(data, expr, input);
            return *this;
        }
        
        ////// = operators  //////
        packet_type& operator=(char * input) { return operator()(input); }
        packet_type& operator=(const char * input) { return operator()(input); }
        packet_type& operator=(const packet_type & input) { return operator()(input.data); }
        
        ////// [] operator  //////
        char & operator[](int i) { return data[i]; }
        
        /// bitshift operators ///
        packet_type& operator<<(const int input) { for (int i = 0; i <= size; i++) data[i] <<= input; }
        packet_type& operator>>(const int input) { for (int i = 0; i <= size; i++) data[i] >>= input; }
        
        ///// cast operators /////
        operator bool() const { return data[0] > 0; }
        operator int() const  { return data[0]; }
        
        // comparison operators //
        bool operator==(const packet_type& to) { return strcmp(data, to.data) == 0; }
        bool operator!=(const packet_type& to) { return strcmp(data, to.data) != 0; }
        bool operator< (const packet_type& to) { return strcmp(data, to.data) <  0; }
        bool operator> (const packet_type& to) { return strcmp(data, to.data) >  0; }
        bool operator<=(const packet_type& to) { return strcmp(data, to.data) <= 0; }
        bool operator>=(const packet_type& to) { return strcmp(data, to.data) >= 0; }
    };
  
#define getsize(x) (sizeof(x)-1)
    typedef packet_type<PACKET_SIZE-1> packet_t;
    typedef packet_type<IDENTIFIER_SIZE> identifier_t;
    typedef packet_type<getsize(packet_t)-getsize(identifier_t)-getsize("+()CSnr")> payload_t;
#undef getsize
    
    // calculates a checksum on a string of data, functions below will handle but provided if required (eg for GPS)
    int calculate_checksum(char * data);
    
    // builds a packet with the inputs given
    packet_t * make_packet(packet_t * to, identifier_t * identifier, payload_t * payload);
    
    // tests to see if a packet has all the right parts matches its checksum
    packet_t * is_valid(packet_t * packet);
    
    // extracts a payload from a packet
    payload_t * get_payload(const packet_t * from, payload_t * to);
    
    // extracts an identifier from a packet
    identifier_t * get_identifier(const packet_t * from, identifier_t * to);
}

#endif /* packet_hpp */
