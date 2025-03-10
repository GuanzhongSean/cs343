#ifndef Q2TALLYVOTES_H
#define Q2TALLYVOTES_H
class Printer;

#if defined(MC)	 // mutex/condition solution
#include "BargingCheckVote.h"
class TallyVotes {
	uOwnerLock mutex;
	uCondLock vlk, blk, dlk;
	unsigned int voters, waiting, groupNo, barging;
	bool signal;
	BCHECK_DECL;

#elif defined(SEM)	// semaphore solution
#include "BargingCheckVote.h"
class TallyVotes {
	uSemaphore mutex, groupSem;
	unsigned int voters, waiting;
	BCHECK_DECL;

#elif defined(BAR)	// barrier solution
_Cormonitor TallyVotes : public uBarrier {
   protected:
	void last();

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
	unsigned int pictureVotes, statueVotes, giftShopVotes, groupSize;
	TourKind tour_kind;
	Printer &printer;
};

#endif	// Q2TALLYVOTES_H
