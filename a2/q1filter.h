#ifndef Q1FILTER_H
#define Q1FILTER_H

#include "q1basefilter.h"

_Coroutine HexFilter : public Filter {
    void main();
    void putHex();
public:
    HexFilter(Filter *next) : Filter(next) {}
};

_Coroutine WhitespaceFilter : public Filter {
    void main();
public:
    WhitespaceFilter(Filter *next) : Filter(next) {}
};

/**
 * CaesarCipherFilter: A filter that performs a Caesar Cipher on the input text.
 * Each character is shifted by a specified number of positions in the alphabet.
 * Non-alphabetic characters (e.g., spaces, punctuation) remain unchanged.
 *
 * Usage: -caesar<shift> (e.g., -caesar3 for a shift of 3)
 *
 * Example:
 *   Input:  "Hello, World!" with a shift of 3
 *   Output: "Khoor, Zruog!"
 */
_Coroutine CaesarCipherFilter : public Filter {
    int shift;  // Number of positions to shift each character
    void main();
public:
    CaesarCipherFilter(Filter *next, int shift) : Filter(next), shift(shift) {}
};

#endif // Q1FILTER_H