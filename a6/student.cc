#include "student.h"

Student::Student(Printer& prt, NameServer& nameServer, WATCardOffice& cardOffice,
				 Groupoff& groupoff, unsigned int id, unsigned int maxPurchases)
	: prt(prt),
	  nameServer(nameServer),
	  cardOffice(cardOffice),
	  groupoff(groupoff),
	  id(id) {
	purchaseNum = prng(1, maxPurchases);
	flavour = static_cast<BottlingPlant::Flavours>(prng(0, 3));
	prt.print(Printer::Kind::Student, id, 'S', flavour, purchaseNum);
}

void Student::main() {
	watcard = cardOffice.create(id, 5);
	giftcard = groupoff.giftCard(id);
	vendingMachine = nameServer.getMachine(id);
	prt.print(Printer::Kind::Student, id, 'V', vendingMachine->getId());
	unsigned int free = 0;
	for (unsigned int purchased = 0; purchased < purchaseNum; purchased++) {
		yield(prng(1, 10));
		WATCard* card;
		while (true) {
			try {
				_Select(giftcard) {
					card = giftcard();
					vendingMachine->buy(flavour, *card);
					giftcard.reset();
					prt.print(Printer::Kind::Student, id, 'G', flavour,
							  card->getBalance());
					break;
				}
				or _When(purchaseNum > 1) _Select(watcard) {
					card = watcard();
					vendingMachine->buy(flavour, *card);
					prt.print(Printer::Kind::Student, id, 'B', flavour,
							  card->getBalance());
					break;
				}
			} catch (WATCardOffice::Lost& le) {
				prt.print(Printer::Kind::Student, id, 'L');
				watcard = cardOffice.create(id, 5);
			} catch (VendingMachine::Stock& se) {
				vendingMachine = nameServer.getMachine(id);
				prt.print(Printer::Kind::Student, id, 'V', vendingMachine->getId());
			} catch (VendingMachine::Funds& fe) {
				watcard = cardOffice.transfer(id, 5 + vendingMachine->cost(), card);
			} catch (VendingMachine::Free& fe) {
				prt.print(Printer::Kind::Student, id, 'A', flavour);
				if (prng(0, 1) == 0) {	// Watch the ad
					yield(4);
				} else {  // Skip the ad
					prt.print(Printer::Kind::Student, id, 'X');
				}
				free++;
			}
		}
	}
	prt.print(Printer::Kind::Student, id, 'F', purchaseNum + free, free);
}
