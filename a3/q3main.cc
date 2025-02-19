#include <cstring>
#include <iostream>

#include "q3buffer.h"
#include "q3consumer.h"
#include "q3producer.h"

using namespace std;

void usage(char* argv[]) {
	cerr << "Usage: " << argv[0]
		 << " [ Cons (> 0) | 'd' (default 5) [ Prods (> 0) | 'd' (default 3) [ "
			"Produce (> 0) | 'd' (default 10) [ BufferSize (> 0) | 'd' "
			"(default 10) [ Delay (> 0) | 'd' (default 5 + 3) [ Processors (> "
			"0) | 'd' (default 1) ] ] ] ] ] ]"
		 << endl;
	exit(1);
}

int main(int argc, char* argv[]) {
	int cons = 5, prods = 3, produce = 10, size = 10, delay = 8, processors = 1;
	struct cmd_error {};
	try {
		if (argc > 7) throw cmd_error();
		if (argc > 6 && strcmp(argv[6], "d") != 0) {
			processors = convert(argv[6]);
		}
		if (argc > 4 && strcmp(argv[4], "d") != 0) {
			size = convert(argv[4]);
		}
		if (argc > 3 && strcmp(argv[3], "d") != 0) {
			produce = convert(argv[3]);
		}
		if (argc > 2 && strcmp(argv[2], "d") != 0) {
			prods = convert(argv[2]);
		}
		if (argc > 1 && strcmp(argv[1], "d") != 0) {
			cons = convert(argv[1]);
		}
		if (argc > 5) {
			if (strcmp(argv[5], "d") == 0) {
				delay = cons + prods;
			} else {
				delay = convert(argv[5]);
			}
		}
		if (cons <= 0 || prods <= 0 || produce <= 0 || size <= 0 ||
			delay <= 0 || processors <= 0) {
			throw cmd_error();
		}
	} catch (...) {
		usage(argv);
	}

	uProcessor p[processors - 1] __attribute__((unused));
	BoundedBuffer<int> buffer(size);
	int subtotals[cons];
	memset(subtotals, 0, sizeof(subtotals));

	Producer* producers[prods];
	for (int i = 0; i < prods; i++) {
		producers[i] = new Producer(buffer, produce, delay);
	}
	Consumer* consumers[cons];
	for (int i = 0; i < cons; i++) {
		consumers[i] = new Consumer(buffer, delay, subtotals[i]);
	}

	for (int i = 0; i < prods; i++) {
		delete producers[i];
	}
	buffer.poison();
	for (int i = 0; i < cons; i++) {
		delete consumers[i];
	}

	int sum = 0;
	for (int i = 0; i < cons; i++) {
		sum += subtotals[i];
	}
	cout << "total: " << sum << endl;

	char* nosummary = getenv("NOSUMMARY");
	if (!nosummary) {
		cerr << endl << "Total Producer/Consumer blocks in insert/remove: "
			 << buffer.blocks() << endl;
		malloc_stats();
	}

	return 0;
}
