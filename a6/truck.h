#ifndef __TRUCK_H__
#define __TRUCK_H__

#include <uPRNG.h>

#include "nameServer.h"
#include "printer.h"

_Task Truck;  // pre-declare
#include "bottlingPlant.h"

_Task Truck {
	static const int numFlavours = 4;  // number of flavours
	Printer & prt;					   // printer to print state info
	NameServer & nameServer;  // nameServer to obtain the location of each vending machine
	BottlingPlant & plant;	  // plant to get shipment
	unsigned int numVendingMachines;  // number of vending machines
	unsigned int maxStockPerFlavour;  // max number of bottles of each flavour
	void main();

   public:
	Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		  unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

#endif
