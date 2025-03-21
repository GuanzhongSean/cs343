#ifndef Q2VOTER_H
#define Q2VOTER_H
#include <uPRNG.h>

#include "q2tallyVotes.h"

_Task Voter {
	unsigned int id, tours;
	TallyVotes & voteTallier;
	Printer & printer;

	void main();
	TallyVotes::Ballot cast() __attribute__((warn_unused_result)) {	 // cast 3-way vote
		// O(1) random selection of 3 items without replacement using divide and conquer.
		static const unsigned int voting[3][2][2] = {
			{{2, 1}, {1, 2}}, {{0, 2}, {2, 0}}, {{0, 1}, {1, 0}}};
		unsigned int picture = prng(3), statue = prng(2);
		return (TallyVotes::Ballot){picture, voting[picture][statue][0],
									voting[picture][statue][1]};
	}

   public:
	enum States : char {
		Start = 'S',
		Vote = 'V',
		Block = 'B',
		Unblock = 'U',
		Barging = 'b',
		Done = 'D',
		Complete = 'C',
		Going = 'G',
		Failed = 'X',
		Terminated = 'T'
	};
	Voter(unsigned int id, unsigned int tours, TallyVotes& voteTallier, Printer& printer);
};

#endif	// Q2VOTER_H
