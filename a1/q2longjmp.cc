#include <unistd.h>

#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

#define MAXDEPTH 1000

#ifdef NOOUTPUT
#define PRINT(stmt)
#else
#define PRINT(stmt) stmt
#endif	// NOOUTPUT

intmax_t eperiod = 100, excepts = 0, calls = 0, dtors = 0, depth = 0;  // counters
PRINT( struct T { ~T() { dtors += 1;
}
}
; )

jmp_buf env[MAXDEPTH];

long int Ackermann(long int m, long int n, long int depth) {
	calls += 1;
	if (m == 0) {
		if (rand() % eperiod <= 2) {
			PRINT(T t;) excepts += 1;
			longjmp(env[depth], 1);
		}
		return n + 1;
	}
	if (n == 0) {
		if (setjmp(env[depth + 1]) == 0) {
			return Ackermann(m - 1, 1, depth + 1);
		} else {
			PRINT(cout << " depth " << depth << " E1 " << m << " " << n << " |");
			if (rand() % eperiod <= 3) {
				PRINT(T t;) excepts += 1;
				longjmp(env[depth], 1);
			}
		}
		PRINT(cout << " E1X " << m << " " << n << endl);
	} else {
		if (setjmp(env[depth + 1]) == 0) {
			return Ackermann(m - 1, Ackermann(m, n - 1, depth + 1), depth + 1);
		} else {
			PRINT(cout << " depth " << depth << " E2 " << m << " " << n << " |");
			if (rand() % eperiod == 0) {
				PRINT(T t;) excepts += 1;
				longjmp(env[depth], 1);
			}
		}
		PRINT(cout << " E2X " << m << " " << n << endl);
	}
	return 0;
}

static intmax_t convert(const char* str) {	// convert C string to integer
	char* endptr;
	errno = 0;								   // reset
	intmax_t val = strtoll(str, &endptr, 10);  // attempt conversion
	if (errno == ERANGE) longjmp(env[0], 1);
	if (endptr == str ||  // conversion failed, no characters generated
		*endptr != '\0')
		longjmp(env[0], 1);	 // not at end of str ?
	return val;
}  // convert

int main(int argc, char* argv[]) {
	volatile intmax_t m = 4, n = 6, seed = getpid();

	if (setjmp(env[0]) == 0) {
		switch (argc) {
			case 5:
				if (strcmp(argv[4], "d") != 0) {  // default ?
					eperiod = convert(argv[4]);
					if (eperiod <= 0) longjmp(env[0], 1);
				}
			case 4:
				if (strcmp(argv[3], "d") != 0) {  // default ?
					seed = convert(argv[3]);
					if (seed <= 0) longjmp(env[0], 1);
				}
			case 3:
				if (strcmp(argv[2], "d") != 0) {  // default ?
					n = convert(argv[2]);
					if (n < 0) longjmp(env[0], 1);
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {  // default ?
					m = convert(argv[1]);
					if (m < 0) longjmp(env[0], 1);
				}
			case 1:
				break;	// use all defaults
			default:
				longjmp(env[0], 1);
		}
	} else {
		cerr << "Usage: " << argv[0]
			 << " [ m (>= 0) | d [ n (>= 0) | d"
				" [ seed (> 0) | d [ eperiod (> 0) | d ] ] ] ]"
			 << endl;
		exit(EXIT_FAILURE);
	}

	srand(seed);  // seed random number
	if (setjmp(env[0]) == 0) {
		PRINT(cout << "Arguments " << m << " " << n << " " << seed << " " << eperiod
				   << endl);
		long int val = Ackermann(m, n, 0);
		PRINT(cout << "Ackermann " << val << endl);
	} else {  // replace
		PRINT(cout << "E3" << endl);
	}
	cout << "calls " << calls << " exceptions " << excepts << " destructors " << dtors
		 << endl;
}
