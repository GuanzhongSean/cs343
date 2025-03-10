#include "q2voter.h"

#include "q2printer.h"

Voter::Voter(unsigned int id, unsigned int tours, TallyVotes &voteTallier,
			 Printer &printer)
	: id(id), tours(tours), voteTallier(voteTallier), printer(printer) {}

void Voter::main() {
	yield(prng() % 20);
	for (unsigned int i = 0; i < tours; i++) {
		PRINT(printer.print(id, Start);)
		try {
			yield(prng() % 5);
			TallyVotes::Tour tour __attribute__((unused)) =
				voteTallier.vote(id, cast());
			yield(prng() % 5);
			PRINT(printer.print(id, Going, tour);)
		} catch (TallyVotes::Failed &) {
			PRINT(printer.print(id, Failed);)
			break;
		}
	}
#if defined(MC) || defined(BAR)
	voteTallier.done(id);
#else
	voteTallier.done();
#endif
	PRINT(printer.print(id, Terminated);)
}
