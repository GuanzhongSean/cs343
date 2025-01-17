#include "q3floatconstanthex.h"
#include <cctype>
#include <cmath>

void FloatConstantHex::main() {
    while (true) {
        buffer += ch; // Accumulate input
        if (buffer.size() == 1 && (buffer[0] == EOT)) { // End of text
            if (validate()) {
                _Resume Match(mantissa * pow(2, exponent));
            } else {
                _Resume Error();
            }
        }
        suspend();
    }
}

bool FloatConstantHex::validate() {
    size_t pos = 0;
    if (!parseHexPrefix(buffer, pos)) return false;
    if (!parseMantissa(buffer, pos)) return false;
    if (!parseExponent(buffer, pos)) return false;
    if (pos != buffer.size()) return false; // Extra characters
    return true;
}

bool FloatConstantHex::parseHexPrefix(const std::string &str, size_t &pos) {
    if (str.substr(pos, 2) == "0x" || str.substr(pos, 2) == "0X") {
        pos += 2;
        return true;
    }
    return false;
}

bool FloatConstantHex::parseMantissa(const std::string &str, size_t &pos) {
    size_t start = pos;
    while (pos < str.size() && std::isxdigit(str[pos])) {
        mantissa = mantissa * 16 + hexDigitToValue(str[pos]);
        pos++;
        mantissaDigits++;
        if (mantissaDigits > 16) return false;
    }
    if (pos < str.size() && str[pos] == '.') {
        pos++;
        hasFraction = true;
        while (pos < str.size() && std::isxdigit(str[pos])) {
            mantissa += hexDigitToValue(str[pos]) / pow(16, mantissaDigits++);
            pos++;
        }
    }
    return pos > start; // At least one digit
}

bool FloatConstantHex::parseExponent(const std::string &str, size_t &pos) {
    if (pos < str.size() && (str[pos] == 'p' || str[pos] == 'P')) {
        pos++;
        bool isNegative = false;
        if (str[pos] == '+' || str[pos] == '-') {
            isNegative = (str[pos] == '-');
            pos++;
        }
        while (pos < str.size() && std::isdigit(str[pos])) {
            exponent = exponent * 10 + (str[pos] - '0');
            pos++;
            exponentDigits++;
            if (exponentDigits > 3) return false;
        }
        if (isNegative) exponent = -exponent;
        return true;
    }
    return false;
}

int FloatConstantHex::hexDigitToValue(char c) {
    if (std::isdigit(c)) return c - '0';
    if (std::isxdigit(c)) return std::tolower(c) - 'a' + 10;
    return -1;
}

void FloatConstantHex::next(char c) {
    ch = c;
    resume(); // Activate coroutine
}
