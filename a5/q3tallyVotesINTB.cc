#include "q3printer.h"
#include "q3tallyVotes.h"

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
					   Printer &printer)
	: groupFull(false),
	  barging(0),
	  ticket_counter(0),
	  serving_ticket(group),
	  voters(voters),
	  waiting(0),
	  groupNo(0),
	  pictureVotes(0),
	  statueVotes(0),
	  giftShopVotes(0),
	  groupSize(group),
	  printer(printer) {}

void TallyVotes::wait() {
	bench.wait();			   // wait until signalled
	while (rand() % 2 == 0) {  // multiple bargers allowed
		try {
			_Accept(vote || done) {	 // accept barging callers
			}
			_Else {	 // do not wait if no callers
			}  // _Accept
		} catch (uMutexFailure::RendezvousFailure &) {
		}
	}  // while
}

void TallyVotes::signalAll() {				// also useful
	while (!bench.empty()) bench.signal();	// drain the condition
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
	VOTER_ENTER(groupSize);
	if (voters < groupSize) _Throw Failed();
	unsigned int my_ticket = ++ticket_counter;
	while (my_ticket > serving_ticket) {
		barging++;
		PRINT(printer.print(id, Voter::Barging, barging, groupNo);)
		wait();
		barging--;
		if (voters < groupSize) _Throw Failed();
	}

	PRINT(printer.print(id, Voter::States::Vote, ballot);)
	pictureVotes += ballot.picture;
	statueVotes += ballot.statue;
	giftShopVotes += ballot.giftshop;
	waiting++;

	if (waiting < groupSize) {
		PRINT(printer.print(id, Voter::States::Block, waiting);)
		wait();
		waiting--;
		PRINT(printer.print(id, Voter::States::Unblock, waiting);)
	} else {
		groupNo++;
		waiting--;
		tour = {(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
					? GiftShop
				: (pictureVotes >= statueVotes) ? Picture
												: Statue,
				groupNo};
		PRINT(printer.print(id, Voter::States::Complete, tour);)
		pictureVotes = statueVotes = giftShopVotes = 0;
		groupFull = true;
		signalAll();
	}
	if (voters < groupSize && !groupFull) _Throw Failed();
	if (waiting == 0) {
		groupFull = false;
		serving_ticket += groupSize;
		signalAll();
	}
	VOTER_LEAVE(groupSize);
	return tour;
}

void TallyVotes::done() {
	voters--;
	if (voters < groupSize) {
		signalAll();
	}
}
