#ifndef Q2PRINTER_H
#define Q2PRINTER_H

#include <iostream>
#include <vector>

#include "q2voter.h"

_Monitor Printer {
	unsigned int numVoters;
	std::vector<std::string> buffer;
	void flush();  // Helper function to flush buffered output

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
