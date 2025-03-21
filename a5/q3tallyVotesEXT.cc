#include "q3printer.h"
#include "q3tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group, Printer &printer)
	: voters(voters),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  group(group),
	  printer(printer) {}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	if (voters < group) _Throw Failed();

	PRINT(printer.print(id, Voter::States::Vote, ballot);)
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < group) {
		PRINT(printer.print(id, Voter::States::Block, waiting);)
		try {
			while (voters >= group) {
				_Accept(done) {
					PRINT(printer.print(id, Voter::States::Done);)
				}
				or _Accept(vote) {
					break;
				}
			}
		} catch (uMutexFailure::RendezvousFailure &) {
		}
		waiting--;
		PRINT(printer.print(id, Voter::States::Unblock, waiting);)
	} else {
		groupNo++;
		waiting--;
		tour = {(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes) ? GiftShop
				: (pictureVotes >= statueVotes)									? Picture
																				: Statue,
				groupNo};
		PRINT(printer.print(id, Voter::States::Complete, tour);)
		pictureVotes = statueVotes = giftShopVotes = 0;
	}
	if (voters < group) _Throw Failed();
	return tour;
}

void TallyVotes::done() {
	voters--;
}
