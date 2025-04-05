#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

#include <uPRNG.h>

#include "printer.h"

_Task NameServer;

_Task BottlingPlant {
   public:
	static constexpr int numFlavours = 4;

   private:
	Printer& prt;
	NameServer& nameServer;
	unsigned int numVendingMachines, maxShippedPerFlavour;
	unsigned int maxStockPerFlavour, timeBetweenShipments;
	bool timeToShutdown;
	unsigned int currentStorage[numFlavours]{0};
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
