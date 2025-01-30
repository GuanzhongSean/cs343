#ifndef Q1FILTER_H
#define Q1FILTER_H

#include "q1basefilter.h"

_Coroutine HexFilter : public Filter {
	void main();
	void putHex(bool eol = false);

   public:
	HexFilter(Filter * next) : Filter(next) {}
};

_Coroutine WhitespaceFilter : public Filter {
	void main();

   public:
	WhitespaceFilter(Filter * next) : Filter(next) {}
};

/**
 * CaesarCipherFilter: A filter that performs a Caesar Cipher on the input text.
 * Each alphabet character is shifted by a specified number of positions.
 * Non-alphabetic characters (e.g., spaces, punctuation) remain unchanged.
 *
 * Usage: -c<shift> (e.g., -c3 for a shift of 3) or -c (default shift of 1)
 *
 * Example:
 *   Input:  "Hello, World!" with a shift of 3
 *   Output: "Khoor, Zruog!"
 */
_Coroutine CaesarCipherFilter : public Filter {
	int shift;	// Number of positions to shift each character
	void main();

   public:
	CaesarCipherFilter(Filter * next, int shift) : Filter(next), shift(shift) {}
};

#endif	// Q1FILTER_H
