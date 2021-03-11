//
//  alphamap.cpp
//  Car
//
//  Created by Jesse on 26/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "alphamap.hpp"
namespace ns_alphamap {
    enum e_alphanum {
        an_unknown = 0,
        an_space,
        an_fullstop,
        an_comma,
        an_dollar,
        an_star,
        an_zero,
        an_one,
        an_two,
        an_three,
        an_four,
        an_five,
        an_six,
        an_seven,
        an_eight,
        an_nine,
        an_A,
        an_B,
        an_C,
        an_D,
        an_E,
        an_F,
        an_G,
        an_H,
        an_I,
        an_J,
        an_K,
        an_L,
        an_M,
        an_N,
        an_P,
        an_Q,
        an_R,
        an_S,
        an_T,
        an_U,
        an_V,
        an_W,
        an_X,
        an_Y,
        an_Z,
        an_plus,
        an_l_bracket,
        an_r_bracket,
        an_colon,
        an_minus,
        // add new charaters here
        an_total
    };

    
#define W {255,255,255},
#define B {0,0,0},
    const unsigned char alphanum[e_alphanum::an_total][8][8][3] {
        {   // unknown
            {B B B B B B B B},
            {B W W W B W W B},
            {B B W B W W B B},
            {B B W B B W B B},
            {B B W B B W B B},
            {B B W W B W B B},
            {B W W B W W W B},
            {B B B B B B B B}
        },{   // space
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B}
        },{   // .
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // ,
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B B W B B B},
            {B B B W B B B B},
            {B B B B B B B B}
        },{   // $
            {B B B W W B B B},
            {B B W W W W B B},
            {B W W B B B B B},
            {B B W W W W B B},
            {B B B B B W W B},
            {B B W W W W B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // *
            {B B B B B B B B},
            {B W B B B W W B},
            {B B W B W W B B},
            {B W W W W W W B},
            {B B W W W B B B},
            {B W W B B W B B},
            {B W B B B B W B},
            {B B B B B B B B}
        },{   // 0
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // 1
            {B B B B B B B B},
            {B B B W W B B B},
            {B B W W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // 2
            {B B B B B B B B},
            {B B W W W W B B},
            {B W B B W W W B},
            {B B B B W W W B},
            {B B W W W W B B},
            {B W W W B B B B},
            {B W W W W W W B},
            {B B B B B B B B}
        },{   // 3
            {B B B B B B B B},
            {B W W W W W B B},
            {B B B B W W W B},
            {B B W W W W B B},
            {B B B B W W W B},
            {B B B B W W W B},
            {B W W W W W B B},
            {B B B B B B B B}
        },{   // 4
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B W W B B},
            {B W B B W W B B},
            {B W B B W W B B},
            {B W W W W W W B},
            {B B B B W W B B},
            {B B B B B B B B}
        },{   // 5
            {B B B B B B B B},
            {B W W W W W B B},
            {B W W B B B B B},
            {B W W W W W B B},
            {B B B B W W W B},
            {B W B B W W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // 6
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B B B B},
            {B W W W W W B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // 7
            {B B B B B B B B},
            {B W W W W W W B},
            {B B B B B W W B},
            {B B B B W W B B},
            {B B B W W B B B},
            {B B W W W B B B},
            {B B W W W B B B},
            {B B B B B B B B}
        },{   // 8
            {B B B B B B B B},
            {B B W W W W B B},
            {B W B B W W W B},
            {B B W W W W B B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // 9
            {B B B B B B B B},
            {B B W W W W B B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B B W W W W W B},
            {B B B B W W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // A
            {B B B B B B B B},
            {B B W W W W B B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B W W W W W W B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B B B B B B B B}
        },{   // B
            {B B B B B B B B},
            {B W W W W W B B},
            {B W W B B W W B},
            {B W W W W W B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B W W W W W B B},
            {B B B B B B B B}
        },{   // C
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B W W B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W B B W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // D
            {B B B B B B B B},
            {B W W W W W B B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B W B B W W W B},
            {B W W W W W B B},
            {B B B B B B B B}
        },{   // E
            {B B B B B B B B},
            {B W W W W W W B},
            {B W W B B B B B},
            {B W W W W W B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W W W W W B},
            {B B B B B B B B}
        },{   // F
            {B B B B B B B B},
            {B W W W W W W B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W W W W B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B B B B B B B B}
        },{   // G
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B W W B},
            {B W W B B B B B},
            {B W W B W W W B},
            {B W W B B W W B},
            {B B W W W W W B},
            {B B B B B B B B}
        },{   // H
            {B B B B B B B B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W W W W W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B B B B B B B B}
        },{   // I
            {B B B B B B B B},
            {B B W W W W B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // J
            {B B B B B B B B},
            {B B B W W W W B},
            {B B B B W W B B},
            {B B B B W W B B},
            {B W W B W W B B},
            {B W W B W W B B},
            {B B W W W B B B},
            {B B B B B B B B}
        },{   // K
            {B B B B B B B B},
            {B W W B B W W B},
            {B W W B W W B B},
            {B W W W W B B B},
            {B W W W W B B B},
            {B W W B W W B B},
            {B W W B B W W B},
            {B B B B B B B B}
        },{   // L
            {B B B B B B B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B W W W W W W B},
            {B B B B B B B B}
        },{   // M
            {B B B B B B B B},
            {B W B B B W W B},
            {B W W B W W W B},
            {B W W W W W W B},
            {B W B W B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B B B B B B B B}
        },{   // N
            {B B B B B B B B},
            {B W B B B W W B},
            {B W W B B W W B},
            {B W W W B W W B},
            {B W B W W W W B},
            {B W B B W W W B},
            {B W B B B W W B},
            {B B B B B B B B}
        },{   // P
            {B B B B B B B B},
            {B W W W W W B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B W W W W W B B},
            {B W W B B B B B},
            {B W W B B B B B},
            {B B B B B B B B}
        },{   // Q
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B B W B},
            {B W W B B B W B},
            {B W W B W B W B},
            {B W W B B W B B},
            {B B W W W B W B},
            {B B B B B B B B}
        },{   // R
            {B B B B B B B B},
            {B W W W W W B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B W W W W W B B},
            {B W W B W B B B},
            {B W W B B W W B},
            {B B B B B B B B}
        },{   // S
            {B B B B B B B B},
            {B B W W W W B B},
            {B W W B B B B B},
            {B B W W W W B B},
            {B B B B W W W B},
            {B W B B W W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // T
            {B B B B B B B B},
            {B W W W W W W B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // U
            {B B B B B B B B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B W W W B},
            {B B W W W W B B},
            {B B B B B B B B}
        },{   // V
            {B B B B B B B B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W W B W W B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // W
            {B B B B B B B B},
            {B W B B B W W B},
            {B W B B B W W B},
            {B W B W B W W B},
            {B W W W W W W B},
            {B W W B W W W B},
            {B W B B B W W B},
            {B B B B B B B B}
        },{   // X
            {B B B B B B B B},
            {B W B B B W W B},
            {B B W B W W B B},
            {B B B W W B B B},
            {B B W W W B B B},
            {B W W B B W B B},
            {B W B B B B W B},
            {B B B B B B B B}
        },{   // Y
            {B B B B B B B B},
            {B W W B B W W B},
            {B W W B B W W B},
            {B B W W W W B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // Z
            {B B B B B B B B},
            {B W W W W W W B},
            {B B B B W W W B},
            {B B B W W W B B},
            {B B W W W B B B},
            {B W W W B B B B},
            {B W W W W W W B},
            {B B B B B B B B}
        },{   // +
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B W B B B},
            {B B B B W B B B},
            {B B W W W W W B},
            {B B B B W B B B},
            {B B B B W B B B},
            {B B B B B B B B}
        },{   // (
            {B B B B B B B B},
            {B B B W W B B B},
            {B B W W B B B B},
            {B B W W B B B B},
            {B B W W B B B B},
            {B B W W B B B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // )
            {B B B B B B B B},
            {B B B W W B B B},
            {B B B B W W B B},
            {B B B B W W B B},
            {B B B B W W B B},
            {B B B B W W B B},
            {B B B W W B B B},
            {B B B B B B B B}
        },{   // :
            {B B B B B B B B},
            {B B B B B B B B},
            {B B W W B B B B},
            {B B W W B B B B},
            {B B B B B B B B},
            {B B W W B B B B},
            {B B W W B B B B},
            {B B B B B B B B}
        },{   // -
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B W W W W W B},
            {B B B B B B B B},
            {B B B B B B B B},
            {B B B B B B B B}
        }
        // add letter pixels here
    };
#undef B
#undef W
}

alphamap::alphamap() : _width(8), _height(8), _depth(3) {
    using namespace ns_alphamap;
    for (int i = 0; i < 256; i++) {
        _asciimap[i] = an_unknown;
    }
    _asciimap[' '] = an_space;
    _asciimap['.'] = an_fullstop;
    _asciimap[','] = an_comma;
    _asciimap['$'] = an_dollar;
    _asciimap['*'] = an_star;
    _asciimap['0'] = an_zero;
    _asciimap['1'] = an_one;
    _asciimap['2'] = an_two;
    _asciimap['3'] = an_three;
    _asciimap['4'] = an_four;
    _asciimap['5'] = an_five;
    _asciimap['6'] = an_six;
    _asciimap['7'] = an_seven;
    _asciimap['8'] = an_eight;
    _asciimap['9'] = an_nine;
    _asciimap['A'] = an_A;
    _asciimap['B'] = an_B;
    _asciimap['C'] = an_C;
    _asciimap['D'] = an_D;
    _asciimap['E'] = an_E;
    _asciimap['F'] = an_F;
    _asciimap['G'] = an_G;
    _asciimap['H'] = an_H;
    _asciimap['I'] = an_I;
    _asciimap['J'] = an_J;
    _asciimap['K'] = an_K;
    _asciimap['L'] = an_L;
    _asciimap['M'] = an_M;
    _asciimap['N'] = an_N;
    _asciimap['O'] = an_zero;
    _asciimap['P'] = an_P;
    _asciimap['Q'] = an_Q;
    _asciimap['R'] = an_R;
    _asciimap['S'] = an_S;
    _asciimap['T'] = an_T;
    _asciimap['U'] = an_U;
    _asciimap['V'] = an_V;
    _asciimap['W'] = an_W;
    _asciimap['X'] = an_X;
    _asciimap['Y'] = an_Y;
    _asciimap['Z'] = an_Z;
    _asciimap['+'] = an_plus;
    _asciimap['('] = an_l_bracket;
    _asciimap[')'] = an_r_bracket;
    _asciimap[':'] = an_colon;
    _asciimap['-'] = an_minus;
    // add mapping here
}
    
void * alphamap::ascii(char input) {
    return (void *) ns_alphamap::alphanum[_asciimap[input]];
}

int alphamap::char_width() { return _width; }
int alphamap::char_height() { return _height; }
int alphamap::char_depth() { return _depth; }
    
