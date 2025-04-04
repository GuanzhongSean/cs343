#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

#include <uPRNG.h>

#include "printer.h"

_Task NameServer;

_Task BottlingPlant {
	static const int numFlavours = 4;
	Printer& prt;
	NameServer& nameServer;
	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	bool timeToShutdown;
	unsigned int currentStorage;
	void main();

   public:
	enum Flavours {
		BlackCherry,
		ClassicCreamSoda,
		RockRootBeer,
		JazzLime
	};	// flavours of soda
	_Exception Shutdown{};
	BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				  unsigned int timeBetweenShipments);
	void getShipment(unsigned int cargo[]);
};

#endif
