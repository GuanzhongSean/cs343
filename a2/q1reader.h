#ifndef Q1READER_H
#define Q1READER_H

#include <iostream>

#include "q1basefilter.h"

using std::istream;

_Coroutine Reader : public Filter {
	istream& in;
	void main();

   public:
	Reader(Filter * next, istream & in) : Filter(next), in(in) {}
};

#endif	// Q1READER_H
