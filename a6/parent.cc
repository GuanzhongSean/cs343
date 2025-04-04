#include "parent.h"

#include <uPRNG.h>

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents,
			   unsigned int parentalDelay)
	: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

void Parent::main() {
	unsigned int total = 0;
	prt.print(Printer::Kind::Parent, 'S');

	while (true) {
		_Accept(~Parent) {
			break;
		}
		_Else {
			yield(parentalDelay);
			unsigned int id = prng(0, numStudents - 1);
			int amount = prng(1, 3);
			bank.deposit(id, amount);
			total += amount;
			prt.print(Printer::Kind::Parent, 'D', id, amount);
		}
	}
	prt.print(Printer::Kind::Parent, 'F', total);
}
