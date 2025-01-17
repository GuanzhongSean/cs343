#ifndef FLOAT_CONSTANT_HEX_H
#define FLOAT_CONSTANT_HEX_H

#include <string>

_Coroutine FloatConstantHex {
    char ch; // character passed by cocaller
    std::string buffer; // buffer for accumulating characters
    double mantissa = 0; // floating-point mantissa
    int exponent = 0; // binary exponent
    bool hasFraction = false;
    int mantissaDigits = 0;
    int exponentDigits = 0;

    void main(); // Coroutine main

    // Parsing and validation helpers
    bool validate();
    bool parseHexPrefix(const std::string &str, size_t &pos);
    bool parseMantissa(const std::string &str, size_t &pos);
    bool parseExponent(const std::string &str, size_t &pos);
    int hexDigitToValue(char c);

public:
    enum { EOT = '\003' }; // End of text character
    _Exception Match {
    public:
        double value;
        Match(double value) : value(value) {}
    };
    _Exception Error {}; // Invalid character detected

    void next(char c); // Send character to coroutine
};

#endif // FLOAT_CONSTANT_HEX_H
