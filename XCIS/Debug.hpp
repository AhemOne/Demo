//
//  Debug.hpp
//  XCIS
//
//  Created by Jesse on 22/10/19.
//  Copyright © 2019 Jesse. All rights reserved.
//
//  note: dont_use expressions are helpers and should not be used directly.
//
//  dprint(fmt, ...) where fmt is a printf format string and ... are the associated parameters. Returns the number of characters printed (essentially works as printf)

#ifndef Debug_hpp
#define Debug_hpp

#include <stdio.h>

// debug print call to be used, calls the lambda expression ahead, returns the number of bytes printed
#define dprint(...) _dsnprintf_dont_use(__VA_ARGS__)()

#ifndef D_ENABLED
//disable debugging, return 0 as no charaters are printed
# define _dsnprintf_dont_use(...) []() -> int { return 0; }
#else
//create lambda expression which calculates a buffer length, declares then fills it with the given format, prints and returns the number of characters printed (i.e mimicks use of normal printf) - captures all local variables by reference to allow access to macro parameters
# define _dsnprintf_dont_use(...) \
    [&]() -> int { \
        char buf[snprintf(NULL, 0, __VA_ARGS__)]; \
        sprintf(buf, __VA_ARGS__); \
        return _print_dont_use(buf);\
    }
# ifdef ARDUINO
#include <arduino.h>
   // Arduino interface for debugging
#  define _print_dont_use(str) Serial.print(str)
# else
   // x64 interface for debugging
#  define _print_dont_use(str) printf("%s", str)
# endif
#endif

#endif /* Debug_hpp */
