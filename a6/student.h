#ifndef __STUDENT_H__
#define __STUDENT_H__

#include <uPRNG.h>

#include "bank.h"
#include "groupoff.h"
#include "nameServer.h"
#include "printer.h"
#include "vendingMachine.h"
#include "watcard.h"
#include "watcardOffice.h"

_Task Student {
	Printer & prt;					  // print state info
	NameServer & nameServer;		  // get vending machine
	WATCardOffice & cardOffice;		  // process watcard
	Groupoff & groupoff;			  // group off to get giftcard
	unsigned int id;				  // id of the student
	unsigned int maxPurchases;		  // max number of bottles to purchase
	unsigned int purchaseNum;		  // number of bottles to purchase
	BottlingPlant::Flavours flavour;  // favourite soda to purchase
	WATCard::FWATCard watcard;		  // from wardcard office
	WATCard::FWATCard giftcard;		  // from groupoff
	VendingMachine *vendingMachine;	  // vending machine to buy soda
	void main();

   public:
	Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
			Groupoff & groupoff, unsigned int id, unsigned int maxPurchases);
};

#endif
