//
//  rational.cpp
//  RationalTest
//
//  Created by Jesse on 1/11/2016.
//  Copyright © 2016 Jesse. All rights reserved.
//

#include "rational.hpp"

int abs(int val) {
    return val > 0 ? val : -val;
}

rat::rat(int numerator, unsigned int denominator) {
    R.numerator = numerator;
    R.denominator = denominator;
    LowestCommonDenominator();
}

//#define __LCD_SETUP__(...) fprintf(stderr, __VA_ARGS__)

#ifndef __LCD_SETUP__
    #define __LCD_SETUP__(...)
#endif

//#define __LCD_LOOP__(...) fprintf(stderr, __VA_ARGS__)

#ifndef __LCD_LOOP__
    #define __LCD_LOOP__(...)
#endif

rational rat::LowestCommonDenominator() {
    if ( R.denominator ) {
        if ( R.numerator ) {
            __LCD_SETUP__("finding LCD of %d/%d.\n", R.numerator, R.denominator);
            int hcf = abs(R.numerator) > R.denominator ? R.denominator : abs(R.numerator);
            __LCD_SETUP__("start from %d.\n", hcf);
            while ( hcf && ( R.numerator % hcf || R.denominator % hcf ) ) {
                __LCD_LOOP__("testing: %d\n",hcf);
                hcf--;
            }
            if ( hcf > 1 ) {
                R.numerator /= hcf;
                R.denominator /= hcf;
            }
        } else {
            R.denominator = 1;
        }
    } else { // on infinity, indicate +ve or -ve infinity
        R.numerator /= abs(R.numerator);
    }
    return R;
}

rational rat::add(rat b) {
    R.numerator *= b.R.denominator;
    b.R.numerator *= R.denominator;
    R.denominator *= b.R.denominator;
    R.numerator += b.R.numerator;
    return LowestCommonDenominator();
} // R + b

rational rat::sub(rat b) {
    R.numerator *= b.R.denominator;
    b.R.numerator *= R.denominator;
    R.denominator *= b.R.denominator;
    R.numerator -= b.R.numerator;
    return LowestCommonDenominator();
} // R + b

rational rat::mul(rat b) {
    R.denominator *= b.R.denominator;
    R.numerator *= b.R.numerator;
    return LowestCommonDenominator();
}

rational rat::div(rat b) {
    if ( R.denominator && b.R.denominator ) {
        R.denominator *= b.R.numerator;
        R.numerator *= b.R.denominator;
    } else {
        R.denominator = 0;
        R.numerator = 0;
    }
    return LowestCommonDenominator();
}

rational rat::mod(rat b) {
    if ( R.denominator && b.R.denominator && R.numerator && b.R.numerator ) {
        R.numerator *= b.R.denominator;
        b.R.numerator *= R.denominator;
        R.denominator *= b.R.denominator;
        int n = (int)(R.numerator / b.R.numerator);
        n *= b.R.numerator;
        R.numerator -= n;
    } else {
        R.denominator = 0;
        R.numerator = 0;
    }
    return LowestCommonDenominator();
}

rational rat::bor(rat b) {
    R.numerator *= b.R.denominator;
    b.R.numerator *= R.denominator;
    R.denominator *= b.R.denominator;
    R.numerator |= b.R.numerator;
    return LowestCommonDenominator();
}

rational rat::band(rat b) {
    R.denominator *= b.R.denominator;
    R.numerator &= b.R.numerator;
    return LowestCommonDenominator();
}

rational rat::bxor(rat b) {
    rat band(*this);
    band.band(b);
    band.binv();
    rat bor(*this);
    bor.bor(b);
    band.band(bor);
    *this = band;
    return LowestCommonDenominator();
}

rational rat::binv() {
    R.numerator = ~R.numerator;
    return this->R;
}

rational rat::lshift(int n) {
    if ( n >= 0 ) {
        while (n) {
            if ((n % 2) || (R.denominator % 2)) {
                R.numerator <<= 1;
            } else {
                R.denominator >>= 1;
            }
            n--;
        }
    } else {
        rshift(-n);
    }
    return LowestCommonDenominator();
}

rational rat::rshift(int n) {
    if ( n >= 0 ) {
        while (n) {
            if ((n % 2) || (R.numerator % 2)) {
                R.denominator <<= 1;
            } else {
                R.numerator >>= 1;
            }
            n--;
        }
    } else {
        lshift(-n);
    }
    return LowestCommonDenominator();
}

rat& rat::operator+=(const rat& right) {
    add(right);
    return *this;
}

rat rat::operator+(const rat& right) {
    rat ret(*this);
    return ret += right;
}

rat& rat::operator++() { // pre inc
    return *this += 1;
}

rat rat::operator++(int) { // post inc
    rat ret(*this);
    *this += 1;
    return ret;
}

rat& rat::operator-=(const rat& right) {
    sub(right);
    return *this;
}

rat rat::operator-(const rat& right) {
    rat ret(*this);
    return ret -= right;
}

rat& rat::operator--() {
    return *this -= 1;
}

rat rat::operator--(int) {
    rat ret(*this);
    *this -= 1;
    return ret;
}

rat& rat::operator*=(const rat& right) {
    mul(right);
    return *this;
}

rat rat::operator*(const rat& right) {
    return rat(*this) *= right;
}

rat& rat::operator/=(const rat& right) {
    div(right);
    return *this;
}

rat rat::operator/(const rat& right) {
    rat ret(*this);
    return ret /= right;
}

rat& rat::operator%=(const rat& right) {
    mod(right);
    return *this;
}
rat rat::operator%(const rat& right) {
    rat ret(*this);
    return ret %= right;
}

rat& rat::operator|=(const rat& right) {
    bor(right);
    return *this;
}

rat rat::operator|(const rat& right) {
    //rat ret(*this);
    return rat(*this) |= right;
}

rat& rat::operator&=(const rat& right) {
    band(right);
    return *this;
}

rat rat::operator&(const rat& right) {
    rat ret(*this);
    return ret &= right;
}

rat& rat::operator^=(const rat& right) {
    bxor(right);
    return *this;
}

rat rat::operator^(const rat& right) {
    rat ret(*this);
    return ret ^= right;
}

rat& rat::operator<<=(const rat& right) {
    lshift(right.R.numerator);
    rshift(right.R.denominator);
    return *this;
}

rat rat::operator<<(const rat& right) {
    return rat(*this) <<= right;
}//

rat& rat::operator>>=(const rat& right) {
    rshift(right.R.numerator);
    lshift(right.R.denominator);
    return *this;
}

rat rat::operator>>(const rat& right) {
    return rat(*this) >>= right;
}//
