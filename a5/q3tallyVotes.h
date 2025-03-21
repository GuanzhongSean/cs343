#ifndef Q3TALLYVOTES_H
#define Q3TALLYVOTES_H

class Printer;

#if defined(EXT)  // external scheduling monitor solution
_Monitor TallyVotes {
#elif defined(INT)	// internal scheduling monitor solution
_Monitor TallyVotes {
	uCondition bench;

#elif defined(INTB)	 // internal scheduling monitor solution with barging
#include "BargingCheckVote.h"

_Monitor TallyVotes {
	bool groupFull;
	unsigned int barging, ticket_counter, serving_ticket;
	uCondition bench;
	void wait();	   // barging version of wait
	void signalAll();  // unblock all waiting tasks
	BCHECK_DECL;

#elif defined(AUTO)	 // automatic-signal monitor solution
#include "AutomaticSignal.h"

_Monitor TallyVotes {
	bool groupFull;
	AUTOMATIC_SIGNAL;

#elif defined(TASK)	 // internal/external scheduling task solution
_Task TallyVotes {
	unsigned int voter_id;
	uCondition bench;
	void main();

#else
#error unsupported voter type
#endif

   public:	// common interface
	_Exception Failed{};
	struct Ballot {
		unsigned int picture, statue, giftshop;
	};
	enum TourKind : char { Picture = 'p', Statue = 's', GiftShop = 'g' };
	struct Tour {
		TourKind tourkind;
		unsigned int groupno;
	};

	TallyVotes(unsigned int voters, unsigned int group, Printer& printer);
	Tour vote(unsigned int id, Ballot ballot);
	void done(
#if defined(TASK)
		unsigned int id
#endif
	);

	// common declarations
   private:
	unsigned int voters, waiting, groupNo, pictureVotes, statueVotes, giftShopVotes,
		group;
	Tour tour;
	Ballot voter_ballot;
	Printer & printer;
};

#endif	// Q3TALLYVOTES_H
