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
	Printer &prt;
	NameServer &nameServer;
	WATCardOffice &cardOffice;
	Groupoff &groupoff;
	unsigned int id, purchaseNum;
	BottlingPlant::Flavours flavour;
	WATCard::FWATCard watcard, giftcard;
	VendingMachine *vendingMachine;
	void main();

   public:
	Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
			Groupoff & groupoff, unsigned int id, unsigned int maxPurchases);
};

#endif
