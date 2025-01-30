#include "q1filter.h"
#include "q1reader.h"
#include "q1writer.h"
#include "q1util.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    vector<string> filterOptions;
    string inputFile, outputFile;

    // Assume filter options come before file names
    int i = 1;
    for (; i < argc && argv[i][0] == '-'; i++) {
        filterOptions.push_back(argv[i]);
    }

    if (i < argc) {
        inputFile = argv[i++];
    }
    if (i < argc) {
        outputFile = argv[i++];
    }

    // Open input and output streams
    istream *in = &cin;
    if (!inputFile.empty()) {
        in = new ifstream(inputFile.c_str());
        if (!*in) {
            cerr << "Error: Cannot open input file " << inputFile << endl;
            return 1;
        }
    }

    ostream *out = &cout;
    if (!outputFile.empty()) {
        out = new ofstream(outputFile.c_str());
        if (!*out) {
            cerr << "Error: Cannot open output file " << outputFile << endl;
            return 1;
        }
    }

    // Create the pipeline in reverse order
    Filter* currentFilter = new Writer(*out);
    for (auto it = filterOptions.rbegin(); it != filterOptions.rend(); it++) {
        if (*it == "-h") {
            currentFilter = new HexFilter(currentFilter);
        } else if (*it == "-w") {
            currentFilter = new WhitespaceFilter(currentFilter);
        } else if (it->find("-c") == 0) {
            enum {DefaultShift = 1};
            int shift = DefaultShift;
            if (it->length() > 2) {
                string shift_arg = it->substr(2);
                try {
                    shift = convert(shift_arg.c_str());
                } catch( invalid_argument& ) {
                    cerr << "Error: Invalid -c[shift] " << shift_arg << endl;
                }
            }
            currentFilter = new CaesarCipherFilter(currentFilter, shift);
        } else {
            cerr << "Error: Unknown filter option " << *it << endl;
            return 1;
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