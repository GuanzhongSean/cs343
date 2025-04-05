#include "bottlingPlant.h"

#include "nameServer.h"
#include "truck.h"

BottlingPlant::BottlingPlant(Printer& prt, NameServer& nameServer,
							 unsigned int numVendingMachines,
							 unsigned int maxShippedPerFlavour,
							 unsigned int maxStockPerFlavour,
							 unsigned int timeBetweenShipments)
	: prt(prt),
	  nameServer(nameServer),
	  numVendingMachines(numVendingMachines),
	  maxShippedPerFlavour(maxShippedPerFlavour),
	  maxStockPerFlavour(maxStockPerFlavour),
	  timeBetweenShipments(timeBetweenShipments),
	  timeToShutdown(false) {
	prt.print(Printer::Kind::BottlingPlant, 'S');
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
	if (timeToShutdown) _Throw Shutdown();
	for (int i = 0; i < numFlavours; i++) {
		cargo[i] = currentStorage[i];
	}
}

void BottlingPlant::main() {
	Truck t(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);

	while (true) {
		unsigned int total = 0;
		for (int i = 0; i < numFlavours; i++) {
			currentStorage[i] = prng(0, maxStockPerFlavour);
			total += currentStorage[i];
		}
		prt.print(Printer::Kind::BottlingPlant, 'G', total);
		yield(timeBetweenShipments);
		_Accept(~BottlingPlant) {
			timeToShutdown = true;
			break;
		}
		or _Accept(getShipment) {
			prt.print(Printer::Kind::BottlingPlant, 'P');
		}
	}

	try {
		_Accept(getShipment);
	} catch (uMutexFailure::RendezvousFailure&) {}

	prt.print(Printer::Kind::BottlingPlant, 'F');
}
