#ifndef Q3PRINTER_H
#define Q3PRINTER_H

#include <iostream>
#include <vector>

#include "q3voter.h"

using namespace std;

#ifdef NOOUTPUT
#define PRINT(stmt)
#else
#define PRINT(stmt) stmt
#endif	// NOOUTPUT

_Monitor Printer {
	struct PrinterEntry {
		bool active = false;
		Voter::States state;
		union {
			TallyVotes::Tour tour;
			TallyVotes::Ballot ballot;
			unsigned int numBlocked;
			struct {
				unsigned int numBlocked2, group;
			};
		};
		enum { NONE, TOUR, BALLOT, BLOCKED, BLOCKED_GROUP } type;
	};

	const unsigned int numVoters;
	PrinterEntry* buffer;
	unsigned int** tours;
	void flush();

   public:
	Printer(unsigned int voters);
	~Printer();
	void print(unsigned int id, Voter::States state);
	void print(unsigned int id, Voter::States state, TallyVotes::Tour tour);
	void print(unsigned int id, Voter::States state, TallyVotes::Ballot vote);
	void print(unsigned int id, Voter::States state, unsigned int numBlocked);
	void print(unsigned int id, Voter::States state, unsigned int numBlocked,
			   unsigned int group);
};

#endif
