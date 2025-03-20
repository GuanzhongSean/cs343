#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"

using namespace std;

void usage(char* argv[]) {
	cerr << "Usage: " << argv[0]
		 << " [ voters (> 0) | 'd' (default 6) [ group (> 0) | 'd' (default 3) "
			"[ tours (> 0) | 'd' (default 1) [ seed (> 0) | 'd' (default "
			"random) [ processors (> 0) | 'd' (default 1) ] ] ] ] ]"
		 << endl;
	exit(1);
}

int main(int argc, char* argv[]) {
	int voters = 6, group = 3, tours = 1, processors = 1;
	struct cmd_error {};
	try {
		if (argc > 6) throw cmd_error();
		if (argc > 5 && strcmp(argv[5], "d") != 0) {
			processors = convert(argv[5]);
		}
		if (argc > 4 && strcmp(argv[4], "d") != 0) {
			set_seed(convert(argv[4]));
		}
		if (argc > 3 && strcmp(argv[3], "d") != 0) {
			tours = convert(argv[3]);
		}
		if (argc > 2 && strcmp(argv[2], "d") != 0) {
			group = convert(argv[2]);
		}
		if (argc > 1 && strcmp(argv[1], "d") != 0) {
			voters = convert(argv[1]);
		}
		if (voters <= 0 || group <= 0 || tours <= 0 || processors <= 0) {
			throw cmd_error();
		}
	} catch (...) {
		usage(argv);
	}

	uProcessor p[processors - 1] __attribute__((unused));

	{
		Printer printer(voters);
		TallyVotes voteTallier(voters, group, printer);
		uArray(Voter, voterTasks, voters);
		for (int i = 0; i < voters; i++) {
			voterTasks[i](i, tours, voteTallier, printer);
		}
	}

	char* nosummary = getenv("NOSUMMARY");
	if (!nosummary) {
		cerr << "Random seed for comparison testing: " << get_seed() << endl;
		malloc_stats();
	}

	return 0;
}
