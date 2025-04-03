#include "parent.h"

#include <uPRNG.h>

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents,
			   unsigned int parentalDelay)
	: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

void Parent::main() {
	prt.print(Printer::Kind::Parent, 'S');	// print start state

	for (;;) {
		_Accept(~Parent) {	// busy waiting for a call to its destructor
			break;
		}
		_Else {
			yield(parentalDelay);  // yield before gift transfered
			unsigned int id =
				prng(0, numStudents - 1);  // student id who will receive gift
			int amount = prng(1, 3);   // amount of money that will be given to student
			bank.deposit(id, amount);  // transfer gift
			prt.print(Printer::Kind::Parent, 'D', id, amount);	// print state info
		}  // _Accept
	}

	prt.print(Printer::Kind::Parent, 'F');	// print finish state
}
