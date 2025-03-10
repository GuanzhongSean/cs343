#include "q2printer.h"

Printer::Printer(unsigned int voters) : numVoters(voters) {
	buffer = new PrinterEntry[numVoters];
	tours = new unsigned int*[numVoters];
	for (unsigned int i = 0; i < numVoters; i++) {
		buffer[i].active = false;
		tours[i] = new unsigned int[3];
		tours[i][0] = tours[i][1] = tours[i][2] = 0;
	}
	PRINT(cout << "V0";
		  for (unsigned int i = 1; i < voters; i++) cout << "\tV" << i;
		  cout << "\n*******";
		  for (unsigned int i = 1; i < voters; i++) cout << "\t*******";
		  cout << endl;)
}

Printer::~Printer() {
	flush();
	delete[] buffer;
	for (unsigned int i = 0; i < numVoters; i++) {
		delete[] tours[i];
	}
	delete[] tours;
	PRINT(cout << "*****************" << endl;
		  cout << "All tours ended" << endl;)
}

void Printer::flush() {
	unsigned int last = 0;
	for (unsigned int i = 0; i < numVoters; i++) {
		if (buffer[i].active) last = i + 1;
	}
	for (unsigned int i = 0; i < last; i++) {
		if (i != 0) cout << "\t";
		if (buffer[i].active) {
			cout << (char)buffer[i].state;
			switch (buffer[i].type) {
				case PrinterEntry::NONE:
					if (buffer[i].state == Voter::States::Terminated) {
						cout << " " << tours[i][0] << "," << tours[i][1] << ","
							 << tours[i][2];
					}
					break;
				case PrinterEntry::TOUR:
					cout << " " << buffer[i].tour.tourkind;
					if (buffer[i].state == Voter::States::Going) {
						cout << " " << buffer[i].tour.groupno;
						switch (buffer[i].tour.tourkind) {
							case TallyVotes::TourKind::Picture:
								tours[i][0]++;
								break;
							case TallyVotes::TourKind::Statue:
								tours[i][1]++;
								break;
							case TallyVotes::TourKind::GiftShop:
								tours[i][2]++;
								break;
						}
					}
					break;
				case PrinterEntry::BALLOT:
					cout << " " << buffer[i].ballot.picture << ","
						 << buffer[i].ballot.statue << ","
						 << buffer[i].ballot.giftshop;
					break;
				case PrinterEntry::BLOCKED:
					cout << " " << buffer[i].numBlocked;
					break;
				case PrinterEntry::BLOCKED_GROUP:
					cout << " " << buffer[i].numBlocked2 << " "
						 << buffer[i].group;
					break;
				default:
					break;
			}
		}
	}
	cout << endl;
	for (unsigned int i = 0; i < last; i++) {
		buffer[i].active = false;
	}
}

void Printer::print(unsigned int id, Voter::States state) {
	if (buffer[id].active) flush();
	buffer[id] = {true, state, {}, PrinterEntry::NONE};
}

void Printer::print(unsigned int id, Voter::States state,
					TallyVotes::Tour tour) {
	if (buffer[id].active) flush();
	buffer[id] = {true, state, {}, PrinterEntry::TOUR};
	buffer[id].tour = tour;
}

void Printer::print(unsigned int id, Voter::States state,
					TallyVotes::Ballot vote) {
	if (buffer[id].active) flush();
	buffer[id] = {true, state, {}, PrinterEntry::BALLOT};
	buffer[id].ballot = vote;
}

void Printer::print(unsigned int id, Voter::States state,
					unsigned int numBlocked) {
	if (buffer[id].active) flush();
	buffer[id] = {true, state, {}, PrinterEntry::BLOCKED};
	buffer[id].numBlocked = numBlocked;
}

void Printer::print(unsigned int id, Voter::States state,
					unsigned int numBlocked, unsigned int group) {
	if (buffer[id].active) flush();
	buffer[id] = {true, state, {}, PrinterEntry::BLOCKED_GROUP};
	buffer[id].numBlocked2 = numBlocked;
	buffer[id].group = group;
}
