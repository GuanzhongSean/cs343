#include "nameServer.h"

#include "vendingMachine.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines,
					   unsigned int numStudents)
	: prt(prt),
	  numVendingMachines(numVendingMachines),
	  numStudents(numStudents),
	  numMachineRegistered(0) {
	prt.print(Printer::Kind::NameServer, 'S');
	positions = new unsigned int[numStudents]();
	for (unsigned int s = 0; s < numStudents; s++) {
		positions[s] = s % numVendingMachines;
	}
	machines = new VendingMachine *[numVendingMachines]();
}

NameServer::~NameServer() {
	delete[] machines;
	delete[] positions;
}

void NameServer::VMregister(VendingMachine *vendingmachine) {
	unsigned int id = vendingmachine->getId();
	machines[id] = vendingmachine;
	prt.print(Printer::Kind::NameServer, 'R', id);
}

VendingMachine *NameServer::getMachine(unsigned int id) {
	unsigned int index = positions[id];
	VendingMachine *vm = machines[index];
	positions[id] = (index + 1) % numVendingMachines;
	prt.print(Printer::Kind::NameServer, 'N', id, vm->getId());
	return vm;
}

VendingMachine **NameServer::getMachineList() {
	return machines;
}

void NameServer::main() {
	while (numMachineRegistered != numVendingMachines) {
		_Accept(VMregister) {
			numMachineRegistered++;
		}
	}
	assert(numMachineRegistered == numVendingMachines);

	while (true) {
		_Accept(~NameServer) {
			break;
		}
		or _Accept(getMachine || getMachineList)
	}
	prt.print(Printer::Kind::NameServer, 'F');
}
