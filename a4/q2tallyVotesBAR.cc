#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: uBarrier(group),
	  votersRemaining(voters),
	  printer(printer),
	  groupSize(group),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;

	block();

	TourKind tour =
		(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
			? GiftShop
		: (pictureVotes >= statueVotes) ? Picture
										: Statue;

	return {tour, id};
}

void TallyVotes::done() {
	votersRemaining--;
	if (votersRemaining < uBarrier::size()) {
		_Throw Failed();
	}
}

void TallyVotes::last() {
	pictureVotes = statueVotes = giftShopVotes = 0;
}
