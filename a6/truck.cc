#include "truck.h"

#include <algorithm>

#include "vendingMachine.h"

using namespace std;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
			 unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
	: prt(prt),
	  nameServer(nameServer),
	  plant(plant),
	  numVendingMachines(numVendingMachines),
	  maxStockPerFlavour(maxStockPerFlavour) {}

void Truck::main() {
	prt.print(Printer::Kind::Truck, 'S');  // starting

	int lastMachineStocked = -1;	  // index of the last machine the truck restocked
	unsigned int cargo[numFlavours];  // cargo to obtain shipment
	VendingMachine **machines =
		nameServer.getMachineList();  // obtain location of each VM from the name server

	while (true) {
		_Accept(~Truck) {
			break;
		}
		_Else {
			yield(prng(1, 10));
			try {
				plant.getShipment(cargo);
			} catch (BottlingPlant::Shutdown &) {
				break;
			}  // try

			// calculate the total amount of sodas
			unsigned int totalSodas = 0;  // total number of sodas
			for (int i = 0; i < numFlavours; i += 1) {
				totalSodas += cargo[i];
			}

			prt.print(Printer::Kind::Truck, 'P', totalSodas);  // picked up shipment

			for (unsigned int cycle = 0; cycle < numVendingMachines;
				 cycle += 1) {				 // restock in cyclic order
				if (totalSodas == 0) break;	 // stop restocking is no more soda

				lastMachineStocked =
					(lastMachineStocked + 1) %
					numVendingMachines;	 // index of next machine to restock

				VendingMachine *vendingmachine =
					machines[lastMachineStocked];  // machine to be restocked
				unsigned id =
					vendingmachine->getId();  // identifier of the vending machine
				unsigned int *stocks =
					vendingmachine->inventory();  // inventory of the machine

				prt.print(Printer::Kind::Truck, 'd', id,
						  totalSodas);	// begin delivery to vending machine

				// restock the vending machine
				unsigned int numNotReplenished = 0;	 // number of bottles not replenished
				for (int f = 0; f < numFlavours; f += 1) {
					unsigned int sodaToRestock =
						min(maxStockPerFlavour - stocks[f], cargo[f]);
					stocks[f] += sodaToRestock;	  // update inventory of vending machine
					cargo[f] -= sodaToRestock;	  // update cargo
					totalSodas -= sodaToRestock;  // update total number of sodas
					numNotReplenished += (maxStockPerFlavour - stocks[f]);
				}

				vendingmachine
					->restocked();	// indicate the truck has finished restocking

				if (numNotReplenished > 0) {  // unsuccessfully filled vending machine
					prt.print(Printer::Kind::Truck, 'U', id, numNotReplenished);
				}

				prt.print(Printer::Kind::Truck, 'D', id, totalSodas);

				if (prng(1, 100) == 1) {
					prt.print(Printer::Kind::Truck, 'W');  // wait for flat tire fix
					yield(10);
				}
			}
		}  // _Accept
	}

	prt.print(Printer::Kind::Truck, 'F');  // finished
}
