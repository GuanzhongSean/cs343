#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

#include "printer.h"

_Task VendingMachine;

_Task NameServer {
	Printer& prt;
	unsigned int numVendingMachines, numStudents, numMachineRegistered;
	unsigned int* positions;
	VendingMachine** machines;

	void main();

   public:
	NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents);
	~NameServer();
	void VMregister(VendingMachine * vendingmachine);
	VendingMachine* getMachine(unsigned int id) __attribute__((warn_unused_result));
	VendingMachine** getMachineList() __attribute__((warn_unused_result));
};

#endif
