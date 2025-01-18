#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "q3floatconstanthex.h"

void processLine(const std::string &line, FloatConstantHex &coroutine) {
	size_t i;
	std::string extraneous("");
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
		std::cout << "\"" << line << "\" : \"" << line.substr(0, i)
				  << "\" yes, value " << std::setprecision(16) << match.value;
	} catch (coroutine.FloatConstantHex::Error &) {
		i++;
		std::cout << "\"" << line << "\" : \"" << line.substr(0, i) << "\" no";
	}
	if (i < line.length()) {
		extraneous = " -- extraneous characters \"" + line.substr(i) + "\"";
	}
	std::cout << extraneous << std::endl;
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
