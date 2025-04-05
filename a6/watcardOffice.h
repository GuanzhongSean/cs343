#ifndef __WATCardOffice_H__
#define __WATCardOffice_H__

#include <list>

#include "bank.h"
#include "printer.h"
#include "watcard.h"

_Task WATCardOffice {
	Printer &prt;
	Bank &bank;
	unsigned int numCouriers;
	struct Job {  // marshalled arguments and return future
		struct Args {
			unsigned int sid;
			unsigned int amount;
			WATCard *card;
			Args(unsigned int sid, unsigned int amount, WATCard *card)
				: sid(sid), amount(amount), card(card) {}
		};
		Args args;				   // call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;  // return future
		Job(Args args) : args(args) {}
	};
	_Task Courier {
		Printer &prt;
		Bank &bank;
		WATCardOffice &cardOffice;
		unsigned lid;
		void main();

	   public:
		Courier(Printer & prt, Bank & bank, WATCardOffice & cardOffice, unsigned int lid);
	};
	friend _Task Courier;
	Courier **courierPool;
	list<Job *> requests;
	list<WATCard *> cardList;
	Job *job;
	void main();

   public:
	_Exception Lost{};	// lost WATCard
	WATCardOffice(Printer & prt, Bank & bank, unsigned int numCouriers);
	~WATCardOffice();
	WATCard::FWATCard create(unsigned int sid, unsigned int amount)
		__attribute__((warn_unused_result));
	WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card)
		__attribute__((warn_unused_result));
	Job *requestWork() __attribute__((warn_unused_result));
};

#endif
