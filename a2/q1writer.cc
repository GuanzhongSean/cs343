#include "q1writer.h"

void Writer::main() {
	try {
		while (true) {
			_Enable;
			out.put(ch);
			count++;
			suspend();
		}
	} catch (Eof &) {
		out << count << " characters" << endl;
	}
}
