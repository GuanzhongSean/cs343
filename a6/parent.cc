#include "parent.h"

#include <uPRNG.h>

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents,
			   unsigned int parentalDelay)
	: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {
	prt.print(Printer::Kind::Parent, 'S');
}

void Parent::main() {
	unsigned int total = 0;
	while (true) {
		_Accept(~Parent) {
			break;
		}
		_Else {
			yield(parentalDelay);
			unsigned int id = prng(numStudents);
			int amount = prng(1, 3);
			bank.deposit(id, amount);
			total += amount;
			prt.print(Printer::Kind::Parent, 'D', id, amount);
		}
	}
	prt.print(Printer::Kind::Parent, 'F', total);
}
