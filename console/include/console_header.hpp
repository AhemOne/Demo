//
//  console_header.hpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef console_header_hpp
#define console_header_hpp

#include <stdio.h>
#include <stdint.h>

namespace console_header {
    #define IMG_WIDTH(x)        static const int image_width = x;
    #define IMG_HEIGHT(x)       static const int image_height = x;
    #define IMG_DATA_BEGIN      static uint8_t startupImage[image_height][image_width][3] {
    #define IMG_DATA_END        };
    #define MSG_START           static const char header_msg[] =
    #define MSG_END             ;

    #define R {0xff,0x00,0x00},
    #define G {0x00,0xff,0x00},
    #define B {0x66,0x66,0xff},
    #define Z {0x00,0x00,0x00},
    #define W {0xFF,0xFF,0xFF},

    #include "../Car/console_header.data"
    // end user data
    #undef MESSAGE
    #undef image_width
    #undef image_height
    #undef IMG_DATA_BEGIN
    #undef IMG_DATA_END
    #undef R
    #undef G
    #undef B
    #undef Z
    #undef W
}

#endif /* console_header_hpp */
