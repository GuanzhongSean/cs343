#include "q1reader.h"

void Reader::main() {
	char c;
	while (in.get(c)) {
		next->put(c);
	}
	_Resume Eof() _At *next;
	next->put(0);  // Arbitrary character to resume next filter
}
