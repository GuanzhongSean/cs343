#ifndef __GROUP_OFF_H__
#define __GROUP_OFF_H__

#include <uPRNG.h>

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
	Printer& prt;
	unsigned int numStudents, sodaCost, groupoffDelay, numGiftCards;
	pair<WATCard::FWATCard, unsigned int>* giftCards;
	WATCard** watCards;
	void main();

   public:
	Groupoff(Printer & prt, unsigned int numStudents, unsigned int sodaCost,
			 unsigned int groupoffDelay);
	~Groupoff();
	WATCard::FWATCard giftCard(unsigned int id);
};

#endif
