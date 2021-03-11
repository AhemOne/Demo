//
//  Helper.hpp
//  XCIS
//
//  Created by Jesse on 26/10/19.
//  Copyright © 2019 Jesse. All rights reserved.
//

#ifndef Helper_hpp
#define Helper_hpp

#ifdef ARDUINO
# include <Arduino.h>
#else
# include <iostream>
#endif

#define name(a) #a

namespace XCIS {
    namespace u_types {
        template <typename type> struct base {
            using type_t = type;
            //static constexpr char type_name[] = name(type);
            //type_t val;
        };
        
        typedef struct : base<char[4]> { } Cookie;
        typedef struct : base<uint8_t> { } CS8;
        typedef struct : base<uint16_t> { } CS16;
        typedef struct : base<uint8_t> { } FarmID;
        typedef struct : base<uint16_t> { } Nonce;
        typedef struct : base<uint16_t> { } DeviceID;
        typedef struct : DeviceID { } FromID;
        typedef struct : DeviceID { } ToID;
    }
    
    template <typename type_t>
    union U {
        type_t t;
        uint8_t c[sizeof(t)];
        
        inline size_t size() { return sizeof(t); }
        U() { }
        U(const type_t & a) { t = a; }
        U(const uint8_t * a) { for (size_t i = 0; i < sizeof(t); i++) c[i] = a[i]; }
    };
    
    template <class ID> union u_t {
        using type_t = typename ID::type_t;
        type_t v;
        uint8_t c[sizeof(v)];
        
        // constructors
        u_t();
        u_t(const type_t & val);
        template <typename new_type_t> u_t(const u_t<new_type_t> & p);
        
        //type_t & base() { return v; }
        
        // assignment
        u_t & operator=(const u_t & val);
        
        // operator functionality
        inline bool operator== (const u_t & val) { return v == val.v; }
        inline bool operator> (const u_t & val) { return v > val.v; }
        bool operator! ();
        
        // increment operators
        u_t & operator++() { v = ((type_t) v) + 1; return *this; }
        
        u_t & operator^=(const u_t & val);
        u_t & operator+=(const u_t & val);
        
        // for checksum
        void calculate(void * data, const size_t number);
        
        // debug
        size_t print();
    };
}

#endif /* Helper_hpp */
