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
	  timeToShutdown(false),
	  currentStorage(0) {}

void BottlingPlant::getShipment(unsigned int cargo[]) {
	if (timeToShutdown)
		_Throw Shutdown();	// in the shutdown case, no shipement is copied
	for (int i = 0; i < numFlavours; i++) {	 // assumming we have 4 flavours
		cargo[i] = currentStorage;			 // put the storage in the crago
	}
	currentStorage = 0;	 // no storage left in the plant
}

void BottlingPlant::main() {
	prt.print(Printer::Kind::BottlingPlant, 'S');  // print starting message
	Truck t(prt, nameServer, *this, numVendingMachines,
			maxStockPerFlavour);  // create a truck

	while (true) {
		if (currentStorage == 0) {
			currentStorage = prng(maxShippedPerFlavour);  // create a production run
			prt.print(Printer::Kind::BottlingPlant, 'G', currentStorage * numFlavours);
		}
		yield(timeBetweenShipments);  // stop for a period of time
		_Accept(~BottlingPlant) {
			timeToShutdown = true;	// set the flag, next getshipment will throw execption
			break;
		}
		or _Accept(getShipment) {
			prt.print(
				Printer::Kind::BottlingPlant,
				'P');  // shipment got by truck, assuming truck can take infinite amount
		}			   //_Accept
	}

	try {
		_Accept(getShipment) {}	 // the last getShipement call, wil throw exception
	} catch (uMutexFailure::RendezvousFailure&) {  // special kind of exception that
												   // indicates problem on task's thread
	}											   // try

	prt.print(Printer::Kind::BottlingPlant, 'F');  // print stopping message
}
