#include "q3floatconstanthex.h"

#include <cctype>
#include <cmath>

void FloatConstantHex::main() {
	if (ch != '0') {
		_Resume Error() _At resumer();
		return;
	}
	suspend();

	if (ch != 'x' && ch != 'X') {
		_Resume Error() _At resumer();
		return;
	}
	suspend();

	bool hasDigits = false, hasFraction = false;
	double mantissa = 0, fractionValue = 0, fractionDivisor = 1;
	while (std::isxdigit(ch) || ch == '.') {
		if (ch == '.') {
			if (hasFraction) {
				_Resume Error() _At resumer();
				return;
			}
			hasFraction = true;
		} else {
			if (hasFraction) {
				fractionDivisor *= 16;
				fractionValue += hexDigitToValue(ch) / fractionDivisor;
			} else {
				mantissa = mantissa * 16 + hexDigitToValue(ch);
			}
			hasDigits = true;
		}
		suspend();
	}
	if (!hasDigits) {
		_Resume Error() _At resumer();
		return;
	}

	int exponent = 0;
	if (ch != 'p' && ch != 'P') {
		_Resume Error() _At resumer();
		return;
	}
	suspend();

	bool isNegativeExponent = false;
	if (ch == '+' || ch == '-') {
		isNegativeExponent = (ch == '-');
		suspend();
	}

	if (!std::isdigit(ch)) {
		_Resume Error() _At resumer();
		return;
	}
	while (std::isdigit(ch)) {
		exponent = exponent * 10 + (ch - '0');
		suspend();
	}
	if (isNegativeExponent) {
		exponent = -exponent;
	}

	if (ch != 'f' && ch != 'l' && ch != 'F' && ch != 'L' && ch != EOT) {
		_Resume Error() _At resumer();
		return;
	} else {
		if (ch != EOT) {
			suspend();
		}
		if (ch == EOT) {
			double result = (mantissa + fractionValue) * std::pow(2, exponent);
			_Resume Match(result) _At resumer();
			return;
		}
	}

	_Resume Error() _At resumer();
	return;
}

int FloatConstantHex::hexDigitToValue(char c) {
	if (std::isdigit(c)) return c - '0';
	if (std::isxdigit(c)) return std::tolower(c) - 'a' + 10;
	return -1;
}
