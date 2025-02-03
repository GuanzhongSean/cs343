#include "q1filter.h"

#include <cctype>
#include <cstdio>

#define HEX_DIGITS 2

void HexFilter::main() {
	enum {
		ChunkNum = 4,
		ChunkInterval = 3
	};	// Number of chunks of a line and spaces between them
	try {
		while (true) {
			_Enable {  // Output the first chunk
				putHex();
				putHex();
				next->put(' ');
				putHex();
				putHex();
			}
			// Output the rest of the chunks
			for (int i = 1; i < ChunkNum; i++) {
				// Output the spaces between chunks
				for (int j = 0; j < ChunkInterval; j++) {
					next->put(' ');
				}
				_Enable {
					putHex();
					putHex();
					next->put(' ');
					putHex();
					putHex(i == ChunkNum - 1);
					// a newline will always be output after the last chunk even
					// EOF is reached
				}
			}
		}
	} catch (Eof &) {
		_Resume Eof() _At *next;
		next->put(0);
	}
}

void HexFilter::putHex(bool eol) {
	char hex[HEX_DIGITS + 1];
	snprintf(hex, HEX_DIGITS + 1, "%02x", ch);
	next->put(hex[0]);
	next->put(hex[1]);
	if (eol) next->put('\n');
	suspend();
}

void WhitespaceFilter::main() {
	try {
		_Enable {
			while (true) {
				while (ch != '\n' && isspace(ch)) {	 // skip leading whitespace
					suspend();
				}
				if (ch == '\n') {  // eliminates empty lines
					suspend();
					continue;
				}
				next->put(ch);	// output the first non-whitespace character
				suspend();
				while (!isspace(ch)) {	// output the rest of the word
					next->put(ch);
					suspend();
				}
				while (true) {
					while (ch != '\n' &&
						   isspace(ch)) {  // skip a block of spaces and tabs
						suspend();
					}
					if (ch == '\n') {  // output a newline if the line ends and
									   // start the next cycle
						next->put(ch);
						break;
					}
					next->put(' ');	 // collapses into a single space if not
									 // end-of-line
					while (!isspace(ch)) {	// output the next word
						next->put(ch);
						suspend();
					}
				}
				suspend();
			}
		}
	} catch (Eof &) {
		_Resume Eof() _At *next;
		next->put(0);
	}
}

void CaesarCipherFilter::main() {
	enum { AlphabetBase = 26 };
	try {
		while (true) {
			_Enable;
			unsigned char c = ch;
			if (isalpha(c)) {
				// Determine the case (uppercase or lowercase)
				char base = isupper(c) ? 'A' : 'a';
				// Apply the shift and wrap around the alphabet
				c = base + (c - base + shift + AlphabetBase) % AlphabetBase;
			}
			next->put(c);
			suspend();
		}
	} catch (Eof &) {
		_Resume Eof() _At *next;
		next->put(0);
	}
}
