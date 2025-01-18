#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "q3floatconstanthex.h"

void processLine(const std::string &line, FloatConstantHex &coroutine) {
	size_t i;
	try {
		for (i = 0; i < line.length(); i++) {
			_Enable {
				coroutine.next(line[i]);
			}
		}
		_Enable {
			coroutine.next(FloatConstantHex::EOT);
		}
	} catch (coroutine.FloatConstantHex::Match &match) {
		std::cout << "\"" << line << "\" : \"" << line << "\" yes, value "
				  << std::setprecision(16) << match.value << std::endl;
	} catch (coroutine.FloatConstantHex::Error &) {
		std::cout << "\"" << line << "\" : \"" << line.substr(0, i + 1)
				  << "\" no";
		std::string extraneous =
			i < line.length() - 1
				? " -- extraneous characters \"" + line.substr(i + 1) + "\""
				: "";
		std::cout << extraneous << std::endl;
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
			std::cout << "\"\" : Warning! Blank line." << std::endl;
			continue;
		}
		FloatConstantHex coroutine;
		processLine(line, coroutine);
	}

	return 0;
}
