#include "q2printer.h"
#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: mutex(1),
	  sync(0),
	  voters(voters),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  group(group),
	  printer(printer) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	mutex.P();
	VOTER_ENTER(group);
	if (voters < group) {
		mutex.V();
		_Throw Failed();
	}

	PRINT(printer.print(id, Voter::States::Vote, ballot);)
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < group) {
		PRINT(printer.print(id, Voter::States::Block, waiting);)
		mutex.V();
		sync.P();
		waiting--;
		PRINT(printer.print(id, Voter::States::Unblock, waiting);)
		if (voters < group) {
			mutex.V();
			_Throw Failed();
		}
	} else {
		groupNo++;
		waiting--;
		tour_kind =
			(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
				? GiftShop
			: (pictureVotes >= statueVotes) ? Picture
											: Statue;
		PRINT(printer.print(id, Voter::States::Complete, Tour{tour_kind, groupNo});)
		pictureVotes = statueVotes = giftShopVotes = 0;
	}

	VOTER_LEAVE(group);
	if (!sync.empty()) {
		sync.V();
	} else {
		mutex.V();
	}
	return {tour_kind, groupNo};
}

void TallyVotes::done() {
	mutex.P();
	voters--;
	if (voters < group && !sync.empty()) {
		sync.V(waiting);
	} else {
		mutex.V();
	}
}
