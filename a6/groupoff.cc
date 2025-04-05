#include "groupoff.h"

#include "watcard.h"

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost,
				   unsigned int groupoffDelay)
	: prt(prt),
	  numStudents(numStudents),
	  sodaCost(sodaCost),
	  groupoffDelay(groupoffDelay),
	  numGiftCards(0) {
	prt.print(Printer::Kind::Groupoff, 'S');
	giftCards = new pair<WATCard::FWATCard, unsigned int>[numStudents]();
	watCards = new WATCard *[numStudents]();
}

Groupoff::~Groupoff() {
	for (unsigned int i = 0; i < numStudents; i++) {
		delete watCards[i];
	}
	delete[] giftCards;
	delete[] watCards;
}

WATCard::FWATCard Groupoff::giftCard(unsigned int id) {
	WATCard::FWATCard w;
	giftCards[numGiftCards] = make_pair(w, id);
	return w;
}

void Groupoff::main() {
	while (numGiftCards < numStudents) {
		_Accept(giftCard) {
			numGiftCards++;
		}
	}

	while (numGiftCards > 0) {
		_Accept(~Groupoff) {
			break;
		}
		_Else {
			yield(groupoffDelay);
			WATCard *w = new WATCard();
			auto [giftCard, studentId] = giftCards[numStudents - numGiftCards];
			numGiftCards--;
			w->deposit(sodaCost);
			watCards[studentId] = w;
			giftCard.delivery(w);
			prt.print(Printer::Kind::Groupoff, 'D', studentId, sodaCost);
		}
	}
	prt.print(Printer::Kind::Groupoff, 'F');
}
