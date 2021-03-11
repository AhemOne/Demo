//
//  Helper.cpp
//  XCIS
//
//  Created by Jesse on 26/10/19.
//  Copyright © 2019 Jesse. All rights reserved.
//

#include "../Helper.hpp"

#define D_ENABLED
#include "../Debug.hpp"

namespace XCIS {
    // constructors
    template <class ID> u_t<ID>::u_t() { };
    template <class ID> u_t<ID>::u_t(const type_t & val) { v = val; }
    
    // assignment operators
    template <class ID> u_t<ID> & u_t<ID>::operator=(const u_t<ID> & val) { v = val.v; return *this; }
    
    // other operators
    template <class ID> u_t<ID> & u_t<ID>::operator^=(const u_t<ID> & val) { v ^= val.v; return *this; }
    template <class ID> bool u_t<ID>::operator! () { return !v; }
    
    // debugging
    template <class ID> size_t u_t<ID>::print() {
        size_t n = dprint("0x");
        for (size_t i = 0; i < sizeof(type_t); i++) {
            uint8_t val = (uint8_t) (0xFF & c[sizeof(type_t) - 1 - i]);
#ifdef ARDUINO_ARCH_AVR
            n += Serial.print(val, HEX);
#else
            n += dprint("%.2X", val);
#endif
        }
        return n;
    }
    
    // cookie specialisation
    template <> u_t<u_types::Cookie>::u_t(const type_t &) { }
    template <> bool u_t<u_types::Cookie>::operator! () { return !(v[0] == 'X' && v[1] == 'C' && v[2] == 'I' && v[3] == 'S'); }
    template <> u_t<u_types::Cookie> & u_t<u_types::Cookie>::operator++() { return *this; }
    template <> u_t<u_types::Cookie> & u_t<u_types::Cookie>::operator=(const u_t<u_types::Cookie> &) { return *this; }
    template <> u_t<u_types::Cookie> & u_t<u_types::Cookie>::operator^=(const u_t<u_types::Cookie> &) { return *this; }
    
    template <> size_t u_t<u_types::Cookie>::print() {
        size_t t = 0;
        for ( size_t i = 0; i < sizeof(type_t); i++, t++ ) t += dprint("%c", v[i]);
        return t;
    }
    
    // checksum calculation specialisation
    template <class type> void calculate_checksum(u_t<type> * me, void * data, const size_t number) {
        using type_t = typename type::type_t;
        
        uint8_t * p = (uint8_t *) data;
        
        me->v = 0;
        for (size_t i = 0; i < number * sizeof(type_t); i++) {
            me->c[i%sizeof(type_t)] ^= p[i];
        }
    }
    
    template <> void u_t<u_types::CS8>::calculate(void * data, const size_t number) { calculate_checksum(this, data, number); }
    template <> void u_t<u_types::CS16>::calculate(void * data, const size_t number) { calculate_checksum(this, data, number); }
    
    // device ID specialisation
    template <> template <> u_t<u_types::FromID>::u_t(const u_t<u_types::DeviceID> & p) { v = p.v; }
    template <> template <> u_t<u_types::ToID>::u_t(const u_t<u_types::DeviceID> & p) { v = p.v; }
    
    template <> template <> u_t<u_types::DeviceID>::u_t(const u_t<u_types::FromID> & p) { v = p.v; }
    template <> template <> u_t<u_types::DeviceID>::u_t(const u_t<u_types::ToID> & p) { v = p.v; }
    
    // instantiations
    template union u_t<u_types::Cookie>;
    template union u_t<u_types::CS8>;
    template union u_t<u_types::CS16>;
    template union u_t<u_types::FarmID>;
    template union u_t<u_types::Nonce>;
    template union u_t<u_types::DeviceID>;
    template union u_t<u_types::FromID>;
    template union u_t<u_types::ToID>;
    
}
