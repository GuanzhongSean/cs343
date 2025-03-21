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

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {
	if (voters < group) _Throw Failed();
	voter_id = id;
	voter_ballot = ballot;
	bench.wait(voter_id);
	if (voters < group) _Throw Failed();
	return tour;
}

void TallyVotes::done(unsigned int id) {
	voter_id = id;
}

void TallyVotes::main() {
	for (;;) {
		try {
			_Accept(done) {
				PRINT(printer.print(voter_id, Voter::States::Done);)
				voters--;
				if (voters < group) {
					while (!bench.empty()) {
						bench.signalBlock();
					}
				}
			}
			or _Accept(vote) {
				PRINT(printer.print(voter_id, Voter::States::Vote, voter_ballot);)
				pictureVotes += voter_ballot.picture;
				statueVotes += voter_ballot.statue;
				giftShopVotes += voter_ballot.giftshop;
				waiting++;
				PRINT(printer.print(voter_id, Voter::States::Block, waiting);)

				if (waiting == group) {
					groupNo++;
					tour = {
						(giftShopVotes >= pictureVotes && giftShopVotes >= statueVotes)
							? GiftShop
						: (pictureVotes >= statueVotes) ? Picture
														: Statue,
						groupNo};
					PRINT(printer.print(voter_id, Voter::States::Complete, tour);)
					pictureVotes = statueVotes = giftShopVotes = 0;

					for (unsigned int i = 0; i < group; i++) {
						waiting--;
						unsigned int cur_id __attribute__((unused)) = bench.front();
						PRINT(printer.print(cur_id, Voter::States::Unblock, waiting);)
						bench.signalBlock();
					}
				}
			}
			or _Accept(~TallyVotes) {
				break;
			}
		} catch (uMutexFailure::RendezvousFailure &) {
		}
	}
}
