#include <uPRNG.h>

#include "config.h"
#include "parent.h"
#include "student.h"

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0]
		 << " [ config-file | 'd' (default file soda.config) [ seed (> 0) | 'd' (default "
			"random) [ processors (> 0) | 'd' (default 1) ] ] ]"
		 << endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	const char *configFile = "soda.config";
	int processors = 1;
	struct cmd_error {};
	try {
		if (argc > 4) throw cmd_error();
		if (argc > 3 && strcmp(argv[3], "d") != 0) {
			processors = convert(argv[3]);
			if (processors <= 0) throw cmd_error();
		}
		if (argc > 2 && strcmp(argv[2], "d") != 0) {
			set_seed(convert(argv[2]));
		}
		if (argc > 1 && strcmp(argv[1], "d") != 0) {
			configFile = argv[1];
		}
	} catch (...) {
		usage(argv);
	}

	uProcessor p[processors - 1] __attribute__((unused));
	ConfigParms configParms;
	processConfigFile(configFile, configParms);

	Printer printer(configParms.numStudents, configParms.numVendingMachines,
					configParms.numCouriers);
	Bank bank(configParms.numStudents);
	Parent parent(printer, bank, configParms.numStudents, configParms.parentalDelay);
	WATCardOffice cardOffice(printer, bank, configParms.numCouriers);
	Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost,
					  configParms.groupoffDelay);
	NameServer nameServer(printer, configParms.numVendingMachines,
						  configParms.numStudents);
	VendingMachine *machines[configParms.numVendingMachines];
	for (unsigned int m = 0; m < configParms.numVendingMachines; m++) {
		machines[m] = new VendingMachine(printer, nameServer, m, configParms.sodaCost);
	}
	BottlingPlant *plant = new BottlingPlant(
		printer, nameServer, configParms.numVendingMachines,
		configParms.maxShippedPerFlavour, configParms.maxStockPerFlavour,
		configParms.timeBetweenShipments);
	Student *students[configParms.numStudents];
	for (unsigned int s = 0; s < configParms.numStudents; s++) {
		students[s] = new Student(printer, nameServer, cardOffice, groupoff, s,
								  configParms.maxPurchases);
	}

	for (unsigned int s = 0; s < configParms.numStudents; s++) {
		delete students[s];
	}
	delete plant;
	for (unsigned int m = 0; m < configParms.numVendingMachines; m++) {
		delete machines[m];
	}

	char *nosummary = getenv("NOSUMMARY");
	if (!nosummary) {
		cerr << "Random seed for comparison testing: " << get_seed() << endl;
		malloc_stats();
	}

	return 0;
}
