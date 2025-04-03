#ifndef __VENDING_MACHINE_H__
#define __VENDING_MACHINE_H__

#include <uPRNG.h>

#include "bottlingPlant.h"
#include "printer.h"
#include "watcard.h"

_Task VendingMachine;  // pre-declare
#include "nameServer.h"

_Task VendingMachine {
	static const int numFlavours = 4;			  // number of flavours
	enum RaiseType { none, funds, stock, free };  // enum type of raise

	Printer & prt;			  // printer to print state info
	NameServer & nameServer;  // nameServer to register VM
	unsigned int id;		  // id for identification
	unsigned int sodaCost;	  // MSRP prive for a bottle of soda
	bool isRestocking;		  // true if the truck is restocking the vending machine
	RaiseType raiseType;	  // flag variable to know when to raise Funds or Stock
	unsigned int
		stocks[numFlavours];  // an array containing the amount of each kind of soda
	uCondition cond;		  // lock to return control from buy
	void main();

   public:
	_Exception Free{};	 // free, advertisement
	_Exception Funds{};	 // insufficient funds
	_Exception Stock{};	 // out of stock for particular flavour
	VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id,
				   unsigned int sodaCost);							// constructor
	void buy(BottlingPlant::Flavours flavour, WATCard & card);		// buy soda
	unsigned int *inventory() __attribute__((warn_unused_result));	// return stocks
	void restocked();				// indicate finish restocking
	_Nomutex unsigned int cost();	// return sodaCost
	_Nomutex unsigned int getId();	// return id
   private:
	// variables for student who is trying to buy
	WATCard *card;					  // student watcard
	BottlingPlant::Flavours flavour;  // flavour the student wanna buy
};

#endif