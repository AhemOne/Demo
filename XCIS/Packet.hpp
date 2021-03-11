//
//  Packet.hpp
//  XCIS
//
//  Copyright © 2019 XCIS. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#ifdef ARDUINO
# include <Arduino.h>
#else
# include <iostream>
#endif

#include "../XCIS.h"
#include "SimpleBuffer.hpp"

namespace XCIS {
    namespace Packet_t {
        template <uint16_t max_length = VM_Parameters::payload_size>
        struct Packet_t : public pointed_simple_buffer<max_length> {
            using buffer_t = pointed_simple_buffer<max_length>;
            
        protected:
            U<packet> data;
            
        public:
            // make empty packet
            Packet_t(bool outgoing = true);
            
            // make packet from payload string
            Packet_t(const uint8_t * payload);
            
            // make packet using incoming stream
            Packet_t(int (*read)(), int (*available)());
            
            // setters
            inline const FarmID & farmID(const FarmID & ID) { return data.t.head.farm = ID; }
            inline const Nonce & nonce(const Nonce & value) { return data.t.head.nonce = value; }
            inline const FromID & from(const FromID & address) { return data.t.head.from = address; }
            inline const ToID & to(const ToID & address) { return data.t.head.to = address; }
            
            // getters
            inline FarmID farmID() { return data.t.head.farm; }
            inline Nonce nonce() { return data.t.head.nonce; }
            inline DeviceID from() { return data.t.head.from; }
            inline DeviceID to() { return data.t.head.to; }
            
            buffer_t * payload_buffer() { return buffer_t::me(); }
            
            inline Packet_t & operator=(const FarmID & id) { data.t.head.farm = id; return *this; }
            inline Packet_t & operator=(const Nonce & nonce) { data.t.head.nonce = nonce; return *this; }
            inline Packet_t & operator=(const FromID & id) { data.t.head.from = id; return *this; }
            inline Packet_t & operator=(const ToID & id) { data.t.head.to = id; return *this; }
            
            inline bool valid() {
                return !(!data.t.head.cookie) && verify_checksum() && payload_size();
            }
            
            // array getters (const)
            uint8_t * payload_data() { return data.t.payload; }
            size_t payload_size() { return buffer_t::size(); }
            uint8_t * packet_data() { return data.c; }
            size_t packet_size() { return payload_size() + sizeof(data.t.head); }
            
            // checksum
            CS8 calculate_checksum() {
                CS8 cs;
                cs.calculate(&data.t.head.farm, packet_size() - sizeof(Cookie) - sizeof(CS8));
                dprint("debug checksum");
                cs.print();
                return cs;
            }
            bool verify_checksum() { return data.t.head.checksum == calculate_checksum(); }
            void set_checksum() { data.t.head.checksum = calculate_checksum(); }
            
            // debugging
            int print() {
                int t = dprint("PACKET INFO:\nRaw data:       0x");
                for (size_t i = 0; i < packet_size(); i++) {
                    uint8_t * const c = packet_data();
                    uint8_t val = (uint8_t) c[i];
#ifdef ARDUINO_ARCH_AVR
                    t += Serial.print(val, HEX);
#else
                    t += dprint("%.2X", val);
#endif
                }
                t += dprint("\n");
                
#ifdef ARDUINO_ARCH_AVR
                Serial.print("Packet size:    ");
                Serial.print(packet_size());
                Serial.print("      Payload size:   ");
                Serial.println(payload_size());
#else
                t += dprint("Packet size:    %lu      Payload size:   %lu\n", packet_size(), payload_size());
#endif
                
                t += dprint("Cookie:         ");
                t += data.t.head.cookie.print();
                t += dprint("    ");
                
                t += dprint("Checksum:       ");
                t += data.t.head.checksum.print();
                t += dprint("\n");
                
                t += dprint("Farm:           ");
                t += data.t.head.farm.print();
                t += dprint("    ");
                
                t += dprint("Nonce:          ");
                t += data.t.head.nonce.print();
                t += dprint("\n");
                
                t += dprint("To:             ");
                t += data.t.head.to.print();
                t += dprint("  ");
                
                t += dprint("From:           ");
                t += data.t.head.from.print();
                t += dprint("\n");
                
                t += dprint("Payload:        \"");
                for (size_t i = 0; i < payload_size(); i++, t++) {
                    uint8_t * const c = payload_data();
#ifdef ARDUINO_ARCH_AVR
                    t += Serial.print((char) c[i]);
#else
                    t += dprint("%c", c[i]);
#endif
                }
                t += dprint("\"\n");
                
                return t;
            }
        };
    }
    typedef Packet_t::Packet_t<> packet_t;
    using packet_t = Packet_t::Packet_t<>;
}

#endif /* Packet_hpp */
