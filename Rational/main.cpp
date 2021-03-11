//
//  main.cpp
//  RationalTest
//
//  Created by Jesse on 1/11/2016.
//  Copyright © 2016 Jesse. All rights reserved.
//

#include <iostream>
#include "rational.hpp"

int main(int argc, const char * argv[]) {
    char string[15];
    
    rat A(6,5);
    rat B(3,1);
    rat C(6,1);
    rat D(1,2);
    
    // addition
    A.printrat();
    B.sprintrat(string);
    printf(" + %s ", string);
    A += B;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    printf("%s + 1 ", string);
    A++;
    A.sprintrat(string);
    printf("= %s\n", string);;
    
    printf("%s + 1 ", string);
    A = A + 1;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    // subtraction
    printf("%s ", string);
    C.sprintrat(string);
    printf("- %s ", string);
    A = A - C;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    printf("%s - 2 ", string);
    A = A - 1;
    A--;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    // division
    printf("%s ", string);
    B.sprintrat(string);
    printf("/ %s ", string);
    A /= B;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    // multiplication
    printf("%s ", string);
    C.sprintrat(string);
    printf("* %s * 9 ", string);
    A = A * C;
    A *= 9;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    // modulo
    A.printrat();
    D.sprintrat(string);
    printf(" %% %s ", string);
    A = A % D;
    A.sprintrat(string);
    printf("= %s\n", string);
    
    rat F(11,1);
    rat G(4,1);
    F.printrat();
    G.sprintrat(string);
    printf(" %% %s ", string);
    F = F % G;
    F.sprintrat(string);
    printf("= %s\n", string);
    
    A.sprintrat(string);
    printf(" A   = %s\n", string);
    (A++).sprintrat(string);
    printf(" H = A++ = %s\n", string);
    A.sprintrat(string);
    printf(" A   = %s\n", string);
    (++A).sprintrat(string);
    printf(" ++A = %s\n", string);
    A.sprintrat(string);
    printf(" A   = %s\n", string);
    
    
    A.printrat();
    printf(" | ");
    B.printrat();
    (A|B).sprintrat(string);
    printf(" = %s\n", string);
    
    A.printrat();
    printf(" & ");
    B.printrat();
    (A&B).sprintrat(string);
    printf(" = %s\n", string);
    
    A.printrat();
    printf(" ^ ");
    B.printrat();
    (A^B).sprintrat(string);
    printf(" = %s\n", string);
    
    A.printrat();
    (A<<=3).sprintrat(string);
    printf(" << 3 = %s\n", string);
    
    A.printrat();
    (A>>=rat(3,2)).sprintrat(string);
    printf(" >> 3/2 = %s\n", string);

    return 0;
}