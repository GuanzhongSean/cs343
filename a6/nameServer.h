#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

#include "printer.h"

_Task VendingMachine;

_Task NameServer {
	Printer& prt;
	unsigned int numVendingMachines;	// number of vending machines
	unsigned int numStudents;			// number of students
	unsigned int numMachineRegistered;	// number of VM registered

	unsigned int* positions;	// array of positions of the VM for students
	VendingMachine** machines;	// array of pointers to vending machines

	void main();

   public:
	NameServer(Printer & prt, unsigned int numVendingMachines, unsigned int numStudents);
	~NameServer();
	void VMregister(VendingMachine * vendingmachine);
	VendingMachine* getMachine(unsigned int id) __attribute__((warn_unused_result));
	VendingMachine** getMachineList() __attribute__((warn_unused_result));
};

#endif
