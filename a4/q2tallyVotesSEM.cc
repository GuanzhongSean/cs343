#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: votersRemaining(voters),
	  groupSize(group),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  quorumFailed(false),
	  printer(printer),
	  mutex(1),
	  sync(0) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	mutex.P();
	VOTER_ENTER(groupSize);

	if (quorumFailed) {
		mutex.V();
		throw Failed();
	}

	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < groupSize) {
		mutex.V();
		sync.P();
		if (quorumFailed) throw Failed();
	} else {
		TourKind tour =
			(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
				? GiftShop
			: (pictureVotes >= statueVotes) ? Picture
											: Statue;
		groupNo++;
		pictureVotes = statueVotes = giftShopVotes = waiting = 0;
		for (unsigned int i = 1; i < groupSize; ++i) sync.V();
		mutex.V();
	}

	VOTER_LEAVE(groupSize);
	return {giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes
				? GiftShop
			: pictureVotes >= statueVotes ? Picture
										  : Statue,
			groupNo};
}

void TallyVotes::done(unsigned int id) {
	mutex.P();
	votersRemaining--;
	if (votersRemaining < groupSize) {
		quorumFailed = true;
		for (unsigned int i = 1; i < groupSize; ++i) sync.V();
	}
	mutex.V();
}
