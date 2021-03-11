//
//  rational.hpp
//  RationalTest
//
//  Created by Jesse on 1/11/2016.
//  Copyright © 2016 Jesse. All rights reserved.
//

#ifndef rational_hpp
#define rational_hpp

#include <stdio.h>

typedef struct Rational {
    int numerator;
    unsigned int denominator;
} rational;

class rat {
public:
    rat(int numerator = 0, unsigned int denominator = 1);

private:
    rational R;
    rational LowestCommonDenominator();
    
    virtual rational add(rat b);
    virtual rational sub(rat b);
    virtual rational mul(rat b);
    virtual rational div(rat b);
    virtual rational mod(rat b);
    
    virtual rational bor(rat b);
    virtual rational band(rat b);
    virtual rational binv();
    virtual rational bxor(rat b);
    
    virtual rational lshift(int n);
    virtual rational rshift(int n);
    
public:
    rational get() { return R; }
    int sprintrat(char * string) { return sprintf(string, "%d/%d", R.numerator, R.denominator); }
    int fprintrat(FILE * stream) { return fprintf(stream, "%d/%d", R.numerator, R.denominator); }
    int printrat() { return printf("%d/%d", R.numerator, R.denominator); }
    float getfloat() { return (float)R.numerator/R.denominator; }
    
    rat& operator+=(const rat& right);
    rat operator+(const rat& right);
    rat& operator++();
    rat operator++(int);
    
    rat& operator-=(const rat& right);
    rat operator-(const rat& right);
    rat& operator--();
    rat operator--(int);
    
    rat& operator*=(const rat& right);
    rat operator*(const rat& right);
    rat& operator<<=(const rat& right); //
    rat operator<<(const rat& right);   //
    
    rat& operator/=(const rat& right);
    rat operator/(const rat& right);
    rat& operator%=(const rat& right);
    rat operator%(const rat& right);
    rat& operator>>=(const rat& right); //
    rat operator>>(const rat& right);   //
    
    rat& operator|=(const rat& right);
    rat operator|(const rat& right);

    rat& operator&=(const rat& right);
    rat operator&(const rat& right);
    
    rat& operator^=(const rat& right);
    rat operator^(const rat& right);

};

#endif /* rational_hpp */
