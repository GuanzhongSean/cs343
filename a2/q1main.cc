#include <cstring>
#include <fstream>
#include <iostream>

#include "q1filter.h"
#include "q1reader.h"
#include "q1writer.h"

using namespace std;

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " [ -filter-options ... ] [ infile [outfile] ]"
		 << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	int i, filterIndex, inputIndex = -1, outputIndex = -1;
	struct cmd_error {};

	for (i = 1; i < argc && argv[i][0] == '-'; i++) {}
	filterIndex = i - 1;
	if (i < argc) {
		inputIndex = i;
		i++;
	}
	if (i < argc) {
		outputIndex = i;
		i++;
	}
	if (i < argc) {
		usage(argv);
	}

	// Open input and output streams
	istream *in = &cin;
	if (inputIndex != -1) {
		try {
			in = new ifstream(argv[inputIndex]);
		} catch (...) {
			cerr << "Error! Could not open input file \"" << argv[inputIndex] << "\""
				 << endl;
			usage(argv);
		}
	}

	ostream *out = &cout;
	if (outputIndex != -1) {
		try {
			out = new ofstream(argv[outputIndex]);
		} catch (...) {
			cerr << "Error! Could not open output file \"" << argv[outputIndex] << "\""
				 << endl;
			usage(argv);
		}
	}

	// Create the pipeline in reverse order
	Filter *currentFilter = new Writer(*out);
	for (int i = filterIndex; i >= 1; i--) {
		try {
			if (strcmp(argv[i], "-h") == 0) {
				currentFilter = new HexFilter(currentFilter);
			} else if (strcmp(argv[i], "-w") == 0) {
				currentFilter = new WhitespaceFilter(currentFilter);
			} else if (strlen(argv[i]) > 1 && argv[i][1] == 'c') {
				enum { DefaultShift = 1 };
				int shift = DefaultShift;
				if (strlen(argv[i]) > 2) {
					const char *shift_arg = argv[i] + 2;
					shift = convert(shift_arg);
				}
				currentFilter = new CaesarCipherFilter(currentFilter, shift);
			} else {
				throw cmd_error{};
			}
		} catch (...) {
			cerr << "Error: invalid command-line option \"" << argv[i] << "\"."
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
