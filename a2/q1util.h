#ifndef Q1UTIL_H
#define Q1UTIL_H

#include <cctype>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#define HEX_DIGITS 2

// Helper function to convert a character to its 2-digit hex representation
inline void charToHex(unsigned char c, char hex[HEX_DIGITS + 1]) {
	snprintf(hex, HEX_DIGITS + 1, "%02x", c);
}

inline intmax_t convert(const char* str) {	// convert C string to integer
	char* endptr;
	errno = 0;								   // reset
	intmax_t val = strtoll(str, &endptr, 10);  // attempt conversion
	if (errno == ERANGE) throw std::out_of_range("");
	if (endptr == str ||  // conversion failed, no characters generated
		*endptr != '\0')
		throw std::invalid_argument("");  // not at end of str ?
	return val;
}  // convert

#endif	// Q1UTIL_H
