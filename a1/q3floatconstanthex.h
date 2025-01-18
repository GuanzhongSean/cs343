#ifndef FLOAT_CONSTANT_HEX_H
#define FLOAT_CONSTANT_HEX_H

#include <string>

_Coroutine FloatConstantHex {
	char ch;
	void main();
	int hexDigitToValue(char c);

   public:
	enum { EOT = '\003' };	// End of text
	_Exception Match {
	   public:
		double value;
		Match(double value) : value(value) {}
	};
	_Exception Error{};

	void next(char c) {
		ch = c;
		resume();
	}
};

#endif	// FLOAT_CONSTANT_HEX_H
