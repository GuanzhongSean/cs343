#include "q1filter.h"
#include "q1basefilter.h"
#include "q1util.h"
#include <iostream>
#include <iomanip>
#include <cctype>

void HexFilter::main() {
    enum { ChunkNum = 4, ChunkInterval = 3 };
    try {
        while (true) {
            _Enable {
                putHex();
                putHex();
                next->put(' ');
                putHex();
                putHex();
            }
            for (int i = 1; i < ChunkNum; i++) {
                for (int j = 0; j < ChunkInterval; j++) {
                    next->put(' ');
                }
                _Enable {
                    putHex();
                    putHex();
                    next->put(' ');
                    putHex();
                    putHex();
                }
            }
            next->put('\n');
        }
    } catch (Eof &) {
        _Resume Eof() _At *next;
        next->put(0);
    }
}

void HexFilter::putHex() {
    char hex[3];
    charToHex(ch, hex);
    next->put(hex[0]);
    next->put(hex[1]);
    suspend();
}

void WhitespaceFilter::main() {
    try {
        bool inWhitespace = false;
        while (true) {
            _Enable;
            unsigned char c = ch;
            if (isblank(c)) {
                if (!inWhitespace && c != '\n') {
                    next->put(' ');
                    inWhitespace = true;
                }
            } else {
                next->put(c);
                inWhitespace = false;
            }
            suspend();
        }
    } catch (Eof &) {
        _Resume Eof() _At *next;
        next->put(0);
    }
}

void CaesarCipherFilter::main() {
    enum {AlphabetBase = 26};
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