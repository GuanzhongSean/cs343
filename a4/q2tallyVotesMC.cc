#include "q2printer.h"
#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: barging(0),
	  signal(false),
	  voters(voters),
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
	if (voters < groupSize) {
		mutex.release();
		_Throw Failed();
	}

	if (signal) {
		barging++;
		printer.print(id, Voter::States::Barging, barging, groupNo);
		blk.wait(mutex);
		barging--;
		signal = false;
		dlk.broadcast();
	}

	printer.print(id, Voter::States::Vote, ballot);
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < groupSize) {
		if (blk.signal()) {
			signal = true;
		} else {
			dlk.broadcast();
		}
		if (voters < groupSize) {
			mutex.release();
			_Throw Failed();
		}
		printer.print(id, Voter::States::Block, waiting);
		vlk.wait(mutex);
		waiting--;
		printer.print(id, Voter::States::Unblock, waiting);
		if (voters < groupSize) {
			mutex.release();
			_Throw Failed();
		}

		if (!vlk.signal()) {
			if (!blk.signal()) {
				signal = false;
				dlk.broadcast();
			}
		}
	} else {
		groupNo++;
		waiting--;
		tour_kind =
			(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
				? GiftShop
			: (pictureVotes >= statueVotes) ? Picture
											: Statue;
		printer.print(id, Voter::States::Complete, Tour{tour_kind, groupNo});
		pictureVotes = statueVotes = giftShopVotes = 0;
		if (vlk.signal()) {
			signal = true;
		}
	}

	VOTER_LEAVE(groupSize);
	mutex.release();
	return {tour_kind, groupNo};
}

void TallyVotes::done(unsigned int id) {
	mutex.acquire();
	if (signal && voters == groupSize) {
		dlk.wait(mutex);
		printer.print(id, Voter::States::Done);
	} else {
		dlk.broadcast();
	}
	voters--;
	if (voters < groupSize) {
		vlk.broadcast();
	}
	mutex.release();
}
