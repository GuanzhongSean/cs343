#include "q2printer.h"
#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group, Printer& printer)
	: uBarrier(group),
	  voters(voters),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  group(group),
	  printer(printer) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	if (voters < group) {
		_Throw Failed();
	}
	if (waiters() == 0) {
		reset(group);
	}
	PRINT(printer.print(id, Voter::States::Vote, ballot);)
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;

	if (waiters() != total() - 1) {
		PRINT(printer.print(id, Voter::Block, waiters() + 1);)
		uBarrier::block();
		PRINT(printer.print(id, Voter::Unblock, waiters());)
	} else {
		uBarrier::block();
		groupNo++;
		tour_kind = (giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
						? GiftShop
					: (pictureVotes >= statueVotes) ? Picture
													: Statue;
		PRINT(printer.print(id, Voter::States::Complete, Tour{tour_kind, groupNo});)
		pictureVotes = statueVotes = giftShopVotes = 0;
	}

	if (voters < group) {
		_Throw Failed();
	}

	return {tour_kind, groupNo};
}

void TallyVotes::done(unsigned int id) {
	voters--;
	if (voters < group && waiters() > 0) {
		block();
		PRINT(printer.print(id, Voter::States::Done);)
	}
}
