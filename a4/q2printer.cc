#include "q2printer.h"

Printer::Printer(unsigned int voters) : numVoters(voters), buffer(voters, "") {
	std::cout << "V0";
	for (unsigned int i = 1; i < voters; ++i) std::cout << "\tV" << i;
	std::cout << "\n*******";
	for (unsigned int i = 1; i < voters; ++i) std::cout << "\t*******";
	std::cout << std::endl;
}

Printer::~Printer() {
	flush();
}

void Printer::flush() {
	bool empty = true;
	for (const auto &entry : buffer) {
		if (!entry.empty()) {
			empty = false;
			break;
		}
	}
	if (empty) return;

	for (const auto &entry : buffer) {
		if (!entry.empty()) std::cout << entry;
		std::cout << "\t";
	}
	std::cout << std::endl;
	buffer.assign(numVoters, "");  // Clear buffer
}

void Printer::print(unsigned int id, Voter::States state) {
	buffer[id] = std::string(1, state);
	flush();
}

void Printer::print(unsigned int id, Voter::States state,
					TallyVotes::Tour tour) {
	buffer[id] = std::string(1, state) + " " + (char)tour.tourkind;
	flush();
}

void Printer::print(unsigned int id, Voter::States state,
					TallyVotes::Ballot vote) {
	buffer[id] = std::string(1, state) + " " + std::to_string(vote.picture) +
				 "," + std::to_string(vote.statue) + "," +
				 std::to_string(vote.giftshop);
	flush();
}

void Printer::print(unsigned int id, Voter::States state,
					unsigned int numBlocked) {
	buffer[id] = std::string(1, state) + " " + std::to_string(numBlocked);
	flush();
}

void Printer::print(unsigned int id, Voter::States state,
					unsigned int numBlocked, unsigned int group) {
	buffer[id] = std::string(1, state) + " " + std::to_string(numBlocked) +
				 " " + std::to_string(group);
	flush();
}
