#include <cstdlib>	// access: rand, srand
#include <cstring>	// access: strcmp
#include <iostream>
using namespace std;
#include <unistd.h>	 // access: getpid

#ifdef NOOUTPUT
#define PRINT(stmt)
#else
#define PRINT(stmt) stmt
#endif	// NOOUTPUT

struct E {};														   // exception type
intmax_t eperiod = 100, excepts = 0, calls = 0, dtors = 0, depth = 0;  // counters
PRINT( struct T { ~T() { dtors += 1;
}
}
; )

long int Ackermann( long int m, long int n, long int depth ) {
	calls += 1;
	if (m == 0) {
		if (rand() % eperiod <= 2) {
			PRINT(T t;) excepts += 1;
			throw E();
		}  // replace
		return n + 1;
	}  // if
	if (n == 0) {
		try {
			return Ackermann(m - 1, 1, depth + 1);	// replace
		} catch (E) {								// replace
			PRINT(cout << " depth " << depth << " E1 " << m << " " << n << " |");
			if (rand() % eperiod <= 3) {
				PRINT(T t;) excepts += 1;
				throw E();
			}  // replace
		}	   // try
		PRINT(cout << " E1X " << m << " " << n << endl);
	} else {
		try {
			return Ackermann(m - 1, Ackermann(m, n - 1, depth + 1),
							 depth + 1);  // replace
		} catch (E) {					  // replace
			PRINT(cout << " depth " << depth << " E2 " << m << " " << n << " |");
			if (rand() % eperiod == 0) {
				PRINT(T t;) excepts += 1;
				throw E();
			}  // replace
		}	   // try
		PRINT(cout << " E2X " << m << " " << n << endl);
	}		   // if
	return 0;  // recover by returning 0
}

static intmax_t convert(const char* str) {	// convert C string to integer
	char* endptr;
	errno = 0;								   // reset
	intmax_t val = strtoll(str, &endptr, 10);  // attempt conversion
	if (errno == ERANGE) throw std::out_of_range("");
	if (endptr == str ||  // conversion failed, no characters generated
		*endptr != '\0')
		throw std::invalid_argument("");  // not at end of str ?
	return val;
}  // convert

int main(int argc, char* argv[]) {
	volatile intmax_t m = 4, n = 6,
					  seed = getpid();	// default values (volatile needed for longjmp)
	struct cmd_error {};				// command-line errors

	try {  // process command-line arguments
		switch (argc) {
			case 5:
				if (strcmp(argv[4], "d") != 0) {  // default ?
					eperiod = convert(argv[4]);
					if (eperiod <= 0) throw cmd_error();
				}
			case 4:
				if (strcmp(argv[3], "d") != 0) {  // default ?
					seed = convert(argv[3]);
					if (seed <= 0) throw cmd_error();
				}
			case 3:
				if (strcmp(argv[2], "d") != 0) {  // default ?
					n = convert(argv[2]);
					if (n < 0) throw cmd_error();
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {  // default ?
					m = convert(argv[1]);
					if (m < 0) throw cmd_error();
				}
			case 1:
				break;	// use all defaults
			default:
				throw cmd_error();
		}  // switch
	} catch (...) {
		cerr << "Usage: " << argv[0]
			 << " [ m (>= 0) | d [ n (>= 0) | d"
				" [ seed (> 0) | d [ eperiod (> 0) | d ] ] ] ]"
			 << endl;
		exit(EXIT_FAILURE);
	}  // try

	srand(seed);  // seed random number
	try {		  // replace
		PRINT(cout << "Arguments " << m << " " << n << " " << seed << " " << eperiod
				   << endl);
		long int val = Ackermann(m, n, 0);
		PRINT(cout << "Ackermann " << val << endl);
	} catch (E) {  // replace
		PRINT(cout << "E3" << endl);
	}  // try
	cout << "calls " << calls << " exceptions " << excepts << " destructors " << dtors
		 << endl;
}
