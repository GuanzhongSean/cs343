#include <uPRNG.h>

#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>

#include "q2quicksort.h"

using namespace std;

void usage(char* argv[]) {
	cerr << "Usage: " << argv[0]
		 << " ( [ unsorted-file | 'd' [ sorted-file | 'd' [ depth (>= 0) ] ] ] "
			"| -t size (>= 0) [ depth (>= 0) ] )"
		 << endl;
	exit(1);
}

// Template function to process a single list of values
template <typename T>
void processLists(istream& input, ostream& output, unsigned int depth) {
	while (true) {
		// Read the number of values in the list
		int count;
		input >> count;
		if (input.eof()) break;
		if (input.fail()) {
			cerr << "Error! Could not read the number of values in the list." << endl;
			exit(1);
		}
		if (count < 0) {
			cerr << "Error! The number of values in the list must be "
					"non-negative: "
				 << count << endl;
			exit(1);
		}

		// Dynamically allocate the array to hold the values
		T* values = new T[count];
		for (int i = 0; i < count; i++) {
			input >> values[i];
			if (input.fail()) {
				cerr << "Error! Could not read value " << i + 1 << " in the list."
					 << endl;
				exit(1);
			}
		}

		// Print the original list
		for (int i = 0; i < count; i++) {
			output << values[i];
			if (i < count - 1) {
				if (i % 22 == 21) {
					output << endl << "  ";
				} else {
					output << " ";
				}
			}
		}
		output << endl;

		// Sort the array using quicksort
		if (count > 1) quicksort(values, 0, count - 1, depth);

		// Print the sorted list
		for (int i = 0; i < count; i++) {
			output << values[i];
			if (i < count - 1) {
				if (i % 22 == 21) {
					output << endl << "  ";
				} else {
					output << " ";
				}
			}
		}
		output << endl << endl;
		delete[] values;
	}
}

int main(int argc, char* argv[]) {
	istream* input = &cin;
	ostream* output = &cout;
	int depth = 0;
	struct cmd_error {};
	try {
		if (argc > 4) throw cmd_error();
		if (argc > 3) depth = convert(argv[3]);
		if (depth < 0) throw cmd_error();
		if (argc > 1 && strcmp(argv[1], "-t") == 0) {
			// Time mode
			PRNG prng;
			intmax_t size = convert(argv[2]);
			if (size < 0) throw cmd_error();
			if (size > UINT_MAX) {
				cerr << "Error! The size of the list larger than the maximum "
						"of unsigned int: "
					 << size << " > " << UINT_MAX << endl;
				exit(1);
			}

			unsigned int usize = static_cast<unsigned int>(size);
			int* values = new int[size];
			for (unsigned int i = 0; i < usize; i++) {
				values[i] = size - i;
			}

			// Randomize values
			unsigned int times = sqrt(usize);
			for (unsigned int counter = 0; counter < times; counter++) {
				swap(values[0], values[prng(size)]);
			}

			// Create additional processors
			uProcessor p[(1 << depth) - 1] __attribute__((unused));

			// Measure sort time
			uTime start = uClock::currTime();
			quicksort(values, 0, size - 1, depth);
			cout << "Sort time " << uClock::currTime() - start << " sec." << endl;

			delete[] values;
		} else {
			// Sort mode
			if (argc > 1 && strcmp(argv[1], "d") != 0) {
				input = new ifstream(argv[1]);
			}
			if (argc > 2 && strcmp(argv[2], "d") != 0) {
				try {
					output = new ofstream(argv[2]);
				} catch (...) {
					cerr << "Error! Could not open sorted output file \"" << argv[2]
						 << "\"" << endl;
					exit(1);
				}
			}

			// Call the appropriate template function based on STYPE
			processLists<STYPE>(*input, *output, depth);
		}
	} catch (...) {
		usage(argv);
	}

	char* nosummary = getenv("NOSUMMARY");
	if (!nosummary) {
		malloc_stats();
	}

	if (input != &cin) delete input;
	if (output != &cout) delete output;

	return 0;
}