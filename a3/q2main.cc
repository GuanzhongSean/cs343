#include <uPRNG.h>

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
		unsigned int count;
		input >> count;
		if (input.fail()) break;

		// Dynamically allocate the array to hold the values
		T* values = new T[count];
		for (unsigned int i = 0; i < count; ++i) {
			input >> values[i];
		}

		// Print the original list
		for (unsigned int i = 0; i < count; ++i) {
			output << values[i] << " ";
			if (i % 23 == 22) {
				output << endl << "  ";
			}
		}
		output << endl;

		// Sort the array using quicksort
		if (count > 1) quicksort(values, 0, count - 1, depth);

		// Print the sorted list
		for (unsigned int i = 0; i < count; ++i) {
			output << values[i] << " ";
			if (i % 23 == 22) {
				output << endl << "  ";
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
		if (argc > 3) depth = convert(argv[3]);
		if (depth < 0) throw cmd_error();
		if (argc > 1 && strcmp(argv[1], "-t") == 0) {
			// Time mode
			PRNG prng;
			int size = convert(argv[2]);
			if (size < 0) throw cmd_error();

			// Dynamically allocate the array
			int* values = new int[size];
			for (int i = 0; i < size; i++) {
				values[i] = size - i;
			}

			// Randomize values
			unsigned int times = sqrt(size);
			for (unsigned int counter = 0; counter < times; counter++) {
				swap(values[0], values[prng(size)]);
			}

			// Create additional processors
			uProcessor p[(1 << depth) - 1] __attribute__((unused));

			// Measure sort time
			uTime start = uClock::currTime();
			quicksort(values, 0, size - 1, depth);
			cout << "Sort time " << uClock::currTime() - start << " sec."
				 << endl;

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
					cerr << "Error! Could not open sorted output file \""
						 << argv[2] << "\"" << endl;
					exit(1);
				}
			}

			// Call the appropriate template function based on STYPE
			processLists<STYPE>(*input, *output, depth);

			if (input != &cin) delete input;
			if (output != &cout) delete output;
		}
	} catch (...) {
		usage(argv);
	}

	char* nosummary = getenv("NOSUMMARY");
	if (!nosummary) {
		malloc_stats();
	}

	return 0;
}