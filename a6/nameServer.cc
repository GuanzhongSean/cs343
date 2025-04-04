#include "nameServer.h"

#include "vendingMachine.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines,
					   unsigned int numStudents)
	: prt(prt),
	  numVendingMachines(numVendingMachines),
	  numStudents(numStudents),
	  numMachineRegistered(0) {
	// distributing the students evenly across the vending machines in a round-robin
	// fashine
	positions = new unsigned int[numStudents];
	for (unsigned int s = 0; s < numStudents; s++) {
		positions[s] = s % numVendingMachines;
	}

	machines = new VendingMachine *[numVendingMachines];
}

NameServer::~NameServer() {
	delete[] machines;
	delete[] positions;
}

void NameServer::VMregister(VendingMachine *vendingmachine) {
	unsigned int id = vendingmachine->getId();		// identifier of the machine
	machines[id] = vendingmachine;					// register the VM
	prt.print(Printer::Kind::NameServer, 'R', id);	// vending machine v registering
}

VendingMachine *NameServer::getMachine(unsigned int id) {
	unsigned int index = positions[id];
	VendingMachine *vendingmachine = machines[index];
	positions[id] = (index + 1) % numVendingMachines;
	prt.print(Printer::Kind::NameServer, 'N', id, vendingmachine->getId());
	return vendingmachine;
}

VendingMachine **NameServer::getMachineList() {
	return machines;
}

void NameServer::main() {
	prt.print(Printer::Kind::NameServer, 'S');	// starting

	for (;;) {	// all vending machines are registered before being given out
		if (numMachineRegistered == numVendingMachines) break;	// finish registering
		_Accept(VMregister) {
			numMachineRegistered++;
		}  // _Accept
	}

	assert(numMachineRegistered == numVendingMachines);	 // prevent unexpected suprises

	for (;;) {
		_Accept(~NameServer) {	// busy waiting for its destructor
			break;
		}
		or
			_Accept(getMachine){

			} or
			_Accept(getMachineList) {}	// _Accept
	}

	prt.print(Printer::Kind::NameServer, 'F');	// finished
}
