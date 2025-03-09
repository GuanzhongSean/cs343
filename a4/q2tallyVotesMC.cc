#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: voters(voters),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  groupSize(group),
	  printer(printer) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	mutex.acquire();
	VOTER_ENTER(groupSize);

	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < groupSize) {
		cond.wait(mutex);
	} else {
		groupNo++;
		pictureVotes = statueVotes = giftShopVotes = waiting = 0;
		cond.broadcast();
	}
	TourKind tour =
		(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
			? GiftShop
		: (pictureVotes >= statueVotes) ? Picture
										: Statue;

	VOTER_LEAVE(groupSize);
	mutex.release();
	return {tour, groupNo};
}

void TallyVotes::done(unsigned int id) {
	mutex.acquire();
	voters--;
	if (voters < groupSize) {
		cond.broadcast();
	}
	mutex.release();
}
