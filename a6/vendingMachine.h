#ifndef __VENDING_MACHINE_H__
#define __VENDING_MACHINE_H__

#include <uPRNG.h>

#include "bottlingPlant.h"
#include "nameServer.h"
#include "printer.h"
#include "watcard.h"

_Task VendingMachine {
	enum RaiseType { none, funds, stock, free };

	Printer &prt;
	NameServer &nameServer;
	unsigned int id, sodaCost;
	bool isRestocking;
	RaiseType raiseType;
	unsigned int stocks[BottlingPlant::numFlavours]{0};
	uCondition cond;
	WATCard *card;
	BottlingPlant::Flavours flavour;
	void main();

   public:
	_Exception Free{};	 // free, advertisement
	_Exception Funds{};	 // insufficient funds
	_Exception Stock{};	 // out of stock for particular flavour
	VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id,
				   unsigned int sodaCost);
	void buy(BottlingPlant::Flavours flavour, WATCard & card);
	unsigned int *inventory() __attribute__((warn_unused_result));
	void restocked();
	_Nomutex unsigned int cost();
	_Nomutex unsigned int getId();
};

#endif
