#include "student.h"

Student::Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice,
				 Groupoff& groupoff, unsigned int id, unsigned int maxPurchases)
	: prt(prt),
	  nameServer(nameServer),
	  cardOffice(cardOffice),
	  groupoff(groupoff),
	  id(id),
	  maxPurchases(maxPurchases) {
	purchaseNum = prng(1, maxPurchases);  // set up purchase number for this student
	flavour = static_cast<BottlingPlant::Flavours>(
		prng(0, 3));					 // select random number for this student
	watcard = cardOffice.create(id, 5);	 // create a $5 watcard
	giftcard = groupoff.giftCard(id);	 // create a giftcard
	prt.print(Printer::Kind::Student, id, 'S', flavour,
			  purchaseNum);	 // let the world know that a student is starting
	vendingMachine =
		nameServer.getMachine(id);	// get vending maching pointer from nameserver
	prt.print(
		Printer::Kind::Student, id, 'V',
		(int)(vendingMachine->getId()));  // print the id of the vending machine we got
}

void Student::main() {
	for (unsigned int purchased = 0; purchased < purchaseNum; purchased++) {
		yield(prng(1, 10));	 // student yield before each ateempat to get a soda
		WATCard* card;
		while (true) {			 // label for prevent using both gift card and
			_Select(giftcard) {	 // if giftcard is avaliable and unused
				try {
					card = giftcard();
					vendingMachine->buy(flavour, *card);
					giftcard.reset();  // reset the giftcard so that if won't be used
					prt.print(Printer::Kind::Student, id, 'G', flavour,
							  (int)card->getBalance());
					break;
				} catch (VendingMachine::Stock& se) {
					vendingMachine =
						nameServer.getMachine(id);	// in this case, student tries to get
													// another vending machine
					prt.print(Printer::Kind::Student, id, 'V',
							  (int)(vendingMachine->getId()));
				} catch (VendingMachine::Free& fe) {
					yield(4);
					prt.print(Printer::Kind::Student, id, 'a', flavour,
							  (int)card->getBalance());
					break;
				}
			}
			or _Select(watcard) {
				try {  // now tries to buy a drink from vending machine
					card = watcard();
					vendingMachine->buy(flavour, *card);
					prt.print(Printer::Kind::Student, id, 'B', flavour,
							  (int)card->getBalance());
					break;
				} catch (
					VendingMachine::Stock& se) {  // vending machine has not enough stock
					vendingMachine =
						nameServer.getMachine(id);	// in this case, student tries to get
													// another vending machine
					prt.print(Printer::Kind::Student, id, 'V',
							  (int)(vendingMachine->getId()));
				} catch (
					VendingMachine::Funds& fe) {  // insufficent funds are in the card.
					watcard = cardOffice.transfer(
						id, 5 + vendingMachine->cost(),
						card);	// get the transfer from watcard office
				} catch (WATCardOffice::Lost& le) {	 // watcard lost
					prt.print(Printer::Kind::Student, id, 'L');
					watcard =
						cardOffice.create(id, 5);  // recreate a watcard from the office
				} catch (VendingMachine::Free& fe) {
					if (prng(2)) {
						// Watch the ad
						yield(4);
						prt.print(Printer::Kind::Student, id, 'A', flavour,
								  (int)card->getBalance());
					} else {
						// Skip the ad
						prt.print(Printer::Kind::Student, id, 'X');
					}
					break;
				}
			}  // _Select
		}  // StartBuying
	}

	// all drinks purchased, student's desitny is fullfilled
	prt.print(Printer::Kind::Student, id, 'F');
}
