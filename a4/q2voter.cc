#include "q2voter.h"

#include "q2printer.h"

Voter::Voter(unsigned int id, unsigned int tours, TallyVotes &voteTallier,
			 Printer &printer)
	: id(id), tours(tours), voteTallier(voteTallier), printer(printer) {}

void Voter::main() {
	for (unsigned int i = 0; i < tours; i++) {
		yield(prng(20));
		printer.print(id, Start);
		yield(prng(5));

		TallyVotes::Ballot ballot = cast();
		printer.print(id, Vote, ballot);
		try {
			TallyVotes::Tour tour = voteTallier.vote(id, ballot);
			printer.print(id, Complete, tour);
			yield(prng(5));
			printer.print(id, Going, tour);
		} catch (TallyVotes::Failed &) {
			printer.print(id, Failed);
			break;
		}
	}
#if defined(MC) || defined(BAR)
	voteTallier.done(id);
#else
	voteTallier.done();
#endif
	printer.print(id, Terminated);
}
