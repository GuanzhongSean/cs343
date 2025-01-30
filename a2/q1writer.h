#ifndef Q1WRITER_H
#define Q1WRITER_H

#include "q1basefilter.h"
#include <iostream>

using ostream = std::ostream;

_Coroutine Writer : public Filter {
    ostream &out;
    unsigned int count = 0;
    void main();
public:
    Writer(ostream &out) : Filter(nullptr), out(out) {}
};

#endif // Q1WRITER_H
