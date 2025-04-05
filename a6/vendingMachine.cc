#include "vendingMachine.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id,
							   unsigned int sodaCost)
	: prt(prt),
	  nameServer(nameServer),
	  id(id),
	  sodaCost(sodaCost),
	  isRestocking(false),
	  raiseType(RaiseType::none) {
	prt.print(Printer::Kind::Vending, id, 'S', sodaCost);
	nameServer.VMregister(this);
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard &card) {
	this->flavour = flavour;
	this->card = &card;

	cond.wait();

	RaiseType rT = raiseType;
	raiseType = RaiseType::none;
	switch (rT) {
		case (RaiseType::funds):
			_Throw Funds();
		case (RaiseType::stock):
			_Throw Stock();
		case (RaiseType::free):
			_Throw Free();
		default:
			break;
	}
}

unsigned int *VendingMachine::inventory() {
	return stocks;
}

void VendingMachine::restocked() {}

unsigned int VendingMachine::cost() {
	return sodaCost;
}

unsigned int VendingMachine::getId() {
	return id;
}

void VendingMachine::main() {
	while (true) {
		_Accept(~VendingMachine) {
			break;
		}
		or _Accept(restocked) {
			isRestocking = false;
			prt.print(Printer::Kind::Vending, id, 'R');
		}
		or _Accept(inventory) {
			isRestocking = true;
			prt.print(Printer::Kind::Vending, id, 'r');
		}
		or _When(!isRestocking) _Accept(buy) {
			if (stocks[flavour] == 0) {
				raiseType = RaiseType::stock;
			} else if (prng(0, 4) == 0) {
				stocks[flavour]--;
				raiseType = RaiseType::free;
				prt.print(Printer::Kind::Vending, id, 'A');
			} else if (card->getBalance() < sodaCost) {
				raiseType = RaiseType::funds;
			} else {
				stocks[flavour]--;
				card->withdraw(sodaCost);
				prt.print(Printer::Kind::Vending, id, 'B', flavour, stocks[flavour]);
			}
			cond.signalBlock();
		}
	}
	prt.print(Printer::Kind::Vending, id, 'F');
}
