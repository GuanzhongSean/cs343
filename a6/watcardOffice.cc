#include "watcardOffice.h"

#include <uPRNG.h>

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers)
	: prt(prt), bank(bank), numCouriers(numCouriers) {
	prt.print(Printer::Kind::WATCardOffice, 'S');
	courierPool = new Courier *[numCouriers]();
	for (unsigned int c = 0; c < numCouriers; c++) {
		courierPool[c] = new Courier(prt, bank, *this, c);
	}
}

WATCardOffice::~WATCardOffice() {
	for (unsigned i = 0; i < numCouriers; i++) {
		delete courierPool[i];
	}
	delete[] courierPool;
	while (!cardList.empty()) {
		delete cardList.front();
		cardList.pop_front();
	}
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
	WATCard *card = new WATCard();
	cardList.push_back(card);
	job = new Job(Job::Args(sid, amount, card));
	prt.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
	return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount,
										  WATCard *card) {
	job = new Job(Job::Args(sid, amount, card));
	prt.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
	return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
	if (requests.empty()) return nullptr;
	Job *job = requests.front();
	requests.pop_front();
	return job;
}

void WATCardOffice::main() {
	while (true) {
		_Accept(~WATCardOffice) {
			requests.clear();
			for (unsigned i = 0; i < numCouriers; i++) {
				_Accept(requestWork);
			}
			break;
		}
		or _When(!requests.empty()) _Accept(requestWork) {
			prt.print(Printer::Kind::WATCardOffice, 'W');
		}
		or _Accept(create || transfer) {
			requests.push_back(job);
		}
	}
	prt.print(Printer::Kind::WATCardOffice, 'F');
}

WATCardOffice::Courier::Courier(Printer &prt, Bank &bank, WATCardOffice &cardOffice,
								unsigned int lid)
	: prt(prt), bank(bank), cardOffice(cardOffice), lid(lid) {
	prt.print(Printer::Kind::Courier, lid, 'S');
}

void WATCardOffice::Courier::main() {
	while (true) {
		Job *job = cardOffice.requestWork();
		if (!job) break;

		WATCard *card = job->args.card;
		unsigned int sid = job->args.sid;
		unsigned int amount = job->args.amount;

		prt.print(Printer::Kind::Courier, lid, 't', sid, amount);
		bank.withdraw(sid, amount);
		card->deposit(amount);

		if (prng(0, 5) == 0) {
			job->result.delivery(new Lost());
			prt.print(Printer::Kind::Courier, lid, 'L', sid);
		} else {
			job->result.delivery(card);
			prt.print(Printer::Kind::Courier, lid, 'T', sid, amount);
		}
		delete job;
	}
	prt.print(Printer::Kind::Courier, lid, 'F');
}
