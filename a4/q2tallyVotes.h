#ifndef Q2TALLYVOTES_H
#define Q2TALLYVOTES_H

class Printer;

#if defined(MC)	 // mutex/condition solution
#include "BargingCheckVote.h"

class TallyVotes {
	uOwnerLock mutex;
	uCondLock vlk, blk, dlk;
	unsigned int barging;
	bool signal;
	BCHECK_DECL;

#elif defined(SEM)	// semaphore solution
#include <uSemaphore.h>

#include "BargingCheckVote.h"

class TallyVotes {
	uSemaphore mutex, sync;
	BCHECK_DECL;

#elif defined(BAR)	// barrier solution
#include <uBarrier.h>

_Cormonitor TallyVotes : public uBarrier {
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

	TallyVotes(unsigned int voters, unsigned int group, Printer &printer);
	Tour vote(unsigned int id, Ballot ballot);
	void done(
#if defined(MC) || defined(BAR)
		unsigned int id
#endif
	);

	// common declarations
   private:
	unsigned int voters, waiting, groupNo, pictureVotes, statueVotes,
		giftShopVotes, group;
	TourKind tour_kind;
	Printer &printer;
};

#endif	// Q2TALLYVOTES_H
