#include "truck.h"

#include "vendingMachine.h"

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
			 unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
	: prt(prt),
	  nameServer(nameServer),
	  plant(plant),
	  numVendingMachines(numVendingMachines),
	  maxStockPerFlavour(maxStockPerFlavour) {
	prt.print(Printer::Kind::Truck, 'S');
}

void Truck::main() {
	int lastMachineStocked = -1;
	unsigned int cargo[BottlingPlant::numFlavours];
	VendingMachine **machines = nameServer.getMachineList();

	while (true) {
		yield(prng(1, 10));
		try {
			plant.getShipment(cargo);
		} catch (BottlingPlant::Shutdown &) {
			break;
		}

		unsigned int totalSodas = 0;
		for (int i = 0; i < BottlingPlant::numFlavours; i++) {
			totalSodas += cargo[i];
		}
		prt.print(Printer::Kind::Truck, 'P', totalSodas);

		for (unsigned int cycle = 0; cycle < numVendingMachines; cycle++) {
			if (totalSodas == 0) break;

			lastMachineStocked = (lastMachineStocked + 1) % numVendingMachines;
			VendingMachine *vendingmachine = machines[lastMachineStocked];
			unsigned id = vendingmachine->getId();
			unsigned int *stocks = vendingmachine->inventory();
			prt.print(Printer::Kind::Truck, 'd', id, totalSodas);

			unsigned int numNotReplenished = 0;
			for (int f = 0; f < BottlingPlant::numFlavours; f++) {
				unsigned int sodaToRestock =
					min(maxStockPerFlavour - stocks[f], cargo[f]);
				stocks[f] += sodaToRestock;
				cargo[f] -= sodaToRestock;
				totalSodas -= sodaToRestock;
				numNotReplenished += (maxStockPerFlavour - stocks[f]);
			}
			vendingmachine->restocked();
			if (numNotReplenished > 0) {
				prt.print(Printer::Kind::Truck, 'U', id, numNotReplenished);
			}
			prt.print(Printer::Kind::Truck, 'D', id, totalSodas);
			if (prng(1, 100) == 1) {
				prt.print(Printer::Kind::Truck, 'W');
				yield(10);
			}
		}
	}
	prt.print(Printer::Kind::Truck, 'F');
}
