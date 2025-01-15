#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

bool ex1_flag = false;
bool ex2_flag = false;
bool ex3_flag = false;

short int ex1_code;
int ex2_code;
long int ex3_code;

intmax_t eperiod = 10000;
int randcnt = 0;
int Rand() {
	randcnt += 1;
	return rand();
}

double rtn1(double i) {
	if (Rand() % eperiod == 0) {
		ex1_flag = true;
		ex1_code = static_cast<short int>(Rand());
		return 0.0;
	}
	return i + Rand();
}

double rtn2(double i) {
	if (Rand() % eperiod == 0) {
		ex2_flag = true;
		ex2_code = Rand();
		return 0.0;
	}
	double result = rtn1(i);
	if (ex1_flag) return 0.0;
	return result + Rand();
}

double rtn3(double i) {
	if (Rand() % eperiod == 0) {
		ex3_flag = true;
		ex3_code = static_cast<long int>(Rand());
		return 0.0;
	}
	double result = rtn2(i);
	if (ex1_flag || ex2_flag) return 0.0;
	return result + Rand();
}

static intmax_t convert(const char *str) {	// convert C string to integer
	char *endptr;
	errno = 0;								   // reset
	intmax_t val = strtoll(str, &endptr, 10);  // attempt conversion
	if (errno == ERANGE) throw std::out_of_range("");
	if (endptr == str ||  // conversion failed, no characters generated
		*endptr != '\0')
		throw std::invalid_argument("");  // not at end of str ?
	return val;
}  // convert

int main(int argc, char *argv[]) {
	intmax_t times = 100000000, seed = getpid();  // default values
	struct cmd_error {};

	try {
		switch (argc) {
			case 4:
				if (strcmp(argv[3], "d") != 0) {  // default ?
					seed = convert(argv[3]);
					if (seed <= 0) throw cmd_error();
				}
			case 3:
				if (strcmp(argv[2], "d") != 0) {  // default ?
					eperiod = convert(argv[2]);
					if (eperiod <= 0) throw cmd_error();
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {  // default ?
					times = convert(argv[1]);
					if (times <= 0) throw cmd_error();
				}
			case 1:
				break;	// use all defaults
			default:
				throw cmd_error();
		}  // switch
	} catch (...) {
		cerr << "Usage: " << argv[0]
			 << " [ times > 0 | d [ eperiod > 0 | d [ seed > 0 | d ] ] ]"
			 << endl;
		exit(EXIT_FAILURE);
	}  // try
	srand(seed);

	double rv = 0.0;
	int ev1 = 0, ev2 = 0, ev3 = 0;
	int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

	for (int i = 0; i < times; i += 1) {
		ex1_flag = ex2_flag = ex3_flag = false;
		double result = rtn3(i);
		if (ex1_flag) {
			ev1 += ex1_code;
			ec1++;
		} else if (ex2_flag) {
			ev2 += ex2_code;
			ec2++;
		} else if (ex3_flag) {
			ev3 += ex3_code;
			ec3++;
		} else {
			rv += result;
			rc++;
		}
	}
	cout << "randcnt " << randcnt << endl;
	cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2
		 << ' ' << ev3 << endl;
	cout << "calls " << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3
		 << endl;
}
