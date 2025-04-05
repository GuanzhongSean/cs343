#ifndef __TRUCK_H__
#define __TRUCK_H__

#include <uPRNG.h>

#include "bottlingPlant.h"
#include "nameServer.h"
#include "printer.h"

_Task Truck {
	Printer& prt;
	NameServer& nameServer;
	BottlingPlant& plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	void main();

   public:
	Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		  unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

#endif
