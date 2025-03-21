#include "AutomaticSignal.h"
#include "q3printer.h"
#include "q3tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: groupFull(false),
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
		EXIT();
		_Throw Failed();
	}

	PRINT(printer.print(id, Voter::States::Vote, ballot);)
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;

	if (waiting + 1 == group) {
		groupNo++;
		tour = {(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
					? GiftShop
				: (pictureVotes >= statueVotes) ? Picture
												: Statue,
				groupNo};
		PRINT(printer.print(id, Voter::States::Complete, tour);)
		pictureVotes = statueVotes = giftShopVotes = 0;
		groupFull = true;
	}

	WAITUNTIL(groupFull || voters < group, waiting++;
			//   cout << "Voters: " << (groupFull || voters < group) << endl;
			  PRINT(printer.print(id, Voter::States::Block, waiting)),
			  waiting--;
			  PRINT(printer.print(id, Voter::States::Unblock, waiting)));
	if (waiting == 0) groupFull = false;
	EXIT();
	if (voters < group) _Throw Failed();
	return tour;
}

void TallyVotes::done() {
	voters--;
	EXIT();
}
