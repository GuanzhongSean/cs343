#include <fstream>
#include <iostream>

#include "q1filter.h"
#include "q1reader.h"
#include "q1writer.h"

using namespace std;

int main(int argc, char *argv[]) {
	int i = 1, filterIndex = -1, inputIndex = -1, outputIndex = -1;
	while (i < argc) {
		if (argv[i][0] != '-') {
			break;
		}
		i++;
	}
	filterIndex = i - 1;
	if (i < argc) {
		inputIndex = i;
		i++;
	}
	if (i < argc) {
		outputIndex = i;
		i++;
	}

	// Open input and output streams
	istream *in = &cin;
	if (inputIndex != -1) {
		in = new ifstream(argv[inputIndex]);
		if (!*in) {
			cerr << "Error: Cannot open input file " << argv[inputIndex]
				 << endl;
			return 1;
		}
	}

	ostream *out = &cout;
	if (outputIndex != -1) {
		out = new ofstream(argv[outputIndex]);
		if (!*out) {
			cerr << "Error: Cannot open output file " << argv[outputIndex]
				 << endl;
			return 1;
		}
	}

	// Create the pipeline in reverse order
	Filter *currentFilter = new Writer(*out);
	for (int i = filterIndex; i >= 1; i--) {
		string arg = argv[i];
		if (arg == "-h") {
			currentFilter = new HexFilter(currentFilter);
		} else if (arg == "-w") {
			currentFilter = new WhitespaceFilter(currentFilter);
		} else if (arg.find("-c") == 0) {
			enum { DefaultShift = 1 };
			int shift = DefaultShift;
			if (arg.length() > 2) {
				string shift_arg = arg.substr(2);
				try {
					shift = convert(shift_arg.c_str());
				} catch (invalid_argument &) {
					cerr << "Error: Invalid -c[shift] " << shift_arg << endl;
				}
			}
			currentFilter = new CaesarCipherFilter(currentFilter, shift);
		} else {
			cerr << "Error: invalid command-line option \"" << arg << "\"."
				 << endl
				 << "Assume all filter options are correctly specified."
				 << endl;
			exit(1);
		}
	}

	// Create the reader and start the process
	Reader reader(currentFilter, *in);
	reader.put(0);

	// Clean up
	if (in != &cin) delete in;
	if (out != &cout) delete out;

	return 0;
}
