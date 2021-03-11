//
//  packet.cpp
//  Car
//
//  Created by Jesse on 30/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//
#include "packet.hpp"

namespace packet {
    int calculate_checksum(char * data) {
        int i = 0, checksum = 0;
        while (data[i] != '\0') checksum ^= data[i++];
        return checksum;
    }
    
    packet_t * make_packet(packet_t * to, identifier_t * identifier, payload_t * payload) {
        int i = sprintf((*to)(), "+%5s(%.70s)", (*identifier)(), (*payload)());
        sprintf(&(*to)[i], "%0.2X\r\n", calculate_checksum((*to)()));
        return to;
    }
    
    packet_t * is_valid(packet_t * packet) {
        if (packet == nullptr) return nullptr;
        char * plus = strchr(((packet_t) *packet)(), '+');
        if (plus == nullptr) return nullptr;
        char * lbrack = strchr(plus, '(');
        if (lbrack == nullptr) return nullptr;
        char * rbrack = strchr(lbrack, ')');
        if (rbrack == nullptr) return nullptr;
        char * creturn = strchr(rbrack, '\r');
        if (creturn == nullptr) return nullptr;
        char * nline = strchr(creturn, '\n');
        if (nline) {
            char test_data[sizeof(packet_t)];
            int i;
            for (i = 0; i < (rbrack - plus) + 1; i++) {
                test_data[i] = (*packet)[i];
                printf("copying %c (%d)\n", (*packet)[i], (*packet)[i]);
            }
            test_data[i] = '\0';
            //strncpy(test_data, ((packet_t) *packet)(), rbrack - plus + 1);
            printf("testing string: %s\n", test_data);
            
            int packet_cs, test_cs = calculate_checksum(test_data);
            sscanf(rbrack + 1, "%X", &packet_cs);
            if (packet_cs == test_cs) {
                return (packet_t *) packet;
            }
        }
        return nullptr;
    }
    
    payload_t * get_payload(const packet_t * from, payload_t * to) {
        if (from == nullptr) return nullptr;
        char * start = strchr(((packet_t) *from)(), '(');
        if (start == nullptr) return nullptr;
        else start++;
        char * end = strchr(start, ')');
        if (end == nullptr) return nullptr;
        strncpy((*to)(), start, end - start);
        return to;
    }
    
    identifier_t * get_identifier(const packet_t * from, identifier_t * to) {
        if (from == nullptr) return nullptr;
        char * start = strchr(((packet_t) *from)(), '+');
        if (start == nullptr) return nullptr;
        else start++;
        char * end = strchr(start, '(');
        if (end == nullptr) return nullptr;
        strncpy((*to)(), start, end - start);
        return to;
    }
}
