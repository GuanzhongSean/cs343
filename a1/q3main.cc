#include "q3floatconstanthex.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

void processLine(const std::string &line, FloatConstantHex &coroutine) {
    try {
        for (char c : line) {
            coroutine.next(c);
        }
        coroutine.next(FloatConstantHex::EOT);
    } catch (FloatConstantHex::Match &match) {
        std::cout << line << " : \"" << line << "\" yes, value "
                  << std::setprecision(16) << match.value << std::endl;
    } catch (FloatConstantHex::Error &) {
        std::cout << line << " : \"" << line << "\" no" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::istream *input = &std::cin;
    std::ifstream file;

    if (argc == 2) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
            return 1;
        }
        input = &file;
    }

    std::string line;
    while (std::getline(*input, line)) {
        if (line.empty()) {
            std::cout << "Warning! Blank line." << std::endl;
            continue;
        }
        FloatConstantHex coroutine;
        processLine(line, coroutine);
    }

    return 0;
}
