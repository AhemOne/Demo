# Rational
## Introduction

Rational is a library developed for using rational numbers (i.e a/b). This was because I wanted to explore and understand operator overloading at the time so no real focus was put finding less time-complex algorithms (the common denominator function is pretty much brute force).

## Files
[Header](rational.hpp)
[Implementation](rational.cpp)
[Tests](main.cpp)

## Usage

**Declare**

```cpp
rat variable_name(numerator, denominator);
```

**Usage**

```cpp
// addition
A += b;
A = B + C;
A++;
++A;

// subtraction
A -= B;
A = B - C;
A--;
--A;

// multiplication
A *= B;
A = B * C;

// division
A /= B;
A = B / C;

// modulo
A = B % C;

// bitwise operation
A = B ^ C;
A = B | C;
A = B & C;
A = B >> C;
A >>= B;
A = B << C;
A <<= C;

// stringize
A.sprintrat(char * buffer)
```
