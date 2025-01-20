#include "q3floatconstanthex.h"

#include <cctype>
#include <cmath>

#define raise_error()              \
	_Resume Error() _At resumer(); \
	return

void FloatConstantHex::main() {
	// Validate '0'
	if (ch != '0') {
		raise_error();
	}
	suspend();	// Wait for next character

	// Validate 'x' or 'X'
	if (ch != 'x' && ch != 'X') {
		raise_error();
	}
	suspend();

	// Validate hexadecimal mantissa
	size_t mantissaDigitNum = 0;
	bool hasDigits = false, hasFraction = false;
	double integral = 0, fractionValue = 0, fractionDivisor = 1;
	while (std::isxdigit(ch) || ch == '.') {
		if (ch == '.') {
			if (hasFraction) {	// Multiple dots not allowed
				raise_error();
			}
			hasFraction = true;
		} else {
			if (hasFraction) {
				mantissaDigitNum++;
				fractionDivisor *= 16;
				fractionValue += hexDigitToValue(ch) / fractionDivisor;
			} else {
				if (mantissaDigitNum > 0 || ch != '0') {
					mantissaDigitNum++;
				}
				integral = integral * 16 + hexDigitToValue(ch);
			}
			if (mantissaDigitNum > 16) {  // Too much digits
				raise_error();
			}
			hasDigits = true;
		}
		suspend();
	}
	if (!hasDigits || mantissaDigitNum > 16) {	// No valid digit
		raise_error();
	}

	// Validate exponent (p or P)
	if (ch != 'p' && ch != 'P') {
		raise_error();
	}
	suspend();

	// Validate exponent sign and value
	bool isNegativeExponent = false;
	if (ch == '+' || ch == '-') {
		isNegativeExponent = (ch == '-');
		suspend();
	}

	int exponent = 0;
	size_t exponentDigitNum = 0;
	hasDigits = false;
	while (std::isdigit(ch)) {
		if (exponentDigitNum > 0 || ch != '0') {
			exponentDigitNum++;
			if (exponentDigitNum > 3) {	 // Too much digits
				raise_error();
			}
		}
		hasDigits = true;
		exponent = exponent * 10 + (ch - '0');
		suspend();
	}
	if (!hasDigits) {  // No valid digit
		raise_error();
	}
	if (isNegativeExponent) {
		exponent = -exponent;
	}

	// Validate optional floating-suffix
	if (ch != 'f' && ch != 'l' && ch != 'F' && ch != 'L' && ch != EOT) {
		raise_error();
	} else {
		if (ch != EOT) {
			suspend();
		}
		// Finalize the result
		double result = (integral + fractionValue) * std::pow(2, exponent);
		_Resume Match(result) _At resumer();
		return;
	}
}

int FloatConstantHex::hexDigitToValue(char c) {
	if (std::isdigit(c)) return c - '0';
	if (std::isxdigit(c)) return std::tolower(c) - 'a' + 10;
	return -1;
}
