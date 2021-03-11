//
//  Packet.cpp
//  XCIS
//
//  Created by Jesse on 22/10/19.
//  Copyright © 2019 Jesse. All rights reserved.
//

#ifdef ARDUINO
# include "../Packet.hpp"
#else
# include "Packet.hpp"
#endif

#define D_ENABLED
#include "../Debug.hpp"


namespace XCIS {
    namespace Packet_t {
        template <uint16_t Max_len> Packet_t<Max_len>::Packet_t(bool outgoing) : buffer_t(data.t.payload) {
            static_assert(Max_len <= MESSAGE_LENGTH, "max_payload_length must be <= MESSAGE_LENGTH");
            if (outgoing) {
                data.t.head.cookie.c[0] = 'X';
                data.t.head.cookie.c[1] = 'C';
                data.t.head.cookie.c[2] = 'I';
                data.t.head.cookie.c[3] = 'S';
            }
            simple_buffer<p_buff<Max_len>>::flush();
        }
        
        template <uint16_t Max_len> Packet_t<Max_len>::Packet_t(const uint8_t * payload) :
            buffer_t(data.t.payload) {
            static_assert(Max_len <= MESSAGE_LENGTH, "max_payload_length must be <= MESSAGE_LENGTH");
            data.t.head.cookie.c[0] = 'X';
            data.t.head.cookie.c[1] = 'C';
            data.t.head.cookie.c[2] = 'I';
            data.t.head.cookie.c[3] = 'S';
            size_t n = 0;
            while(payload[n] && n < VM_Parameters::payload_size) this->write(payload[n++]);
            this->rewind();
        }
        
        template <uint16_t Max_len> Packet_t<Max_len>::Packet_t(int (*read)(), int (*available)()) : simple_buffer<p_buff<Max_len>>(data.t.payload) {
            const size_t N = (size_t) available();
            //dprint("reading from stream: %d characters available: ", N);
            size_t i = 0;
            for (; i < sizeof(header); i++) data.c[i] = read();
            for (; i < N; i++) this->write(read());
            //dprint("%s\n", data.c);
            this->rewind();
        }
    }
    
    template struct Packet_t::Packet_t<>;
}
