#include "bank.h"

Bank::Bank(unsigned int numStudents) {
	accounts =
		new unsigned int[numStudents];	// initialize array to store student-account info
	for (unsigned int i = 0; i < numStudents; i++) {
		accounts[i] = 0;  // each account initially starts with a balance of $0
	}
}

Bank::~Bank() {
	delete accounts;  // free memory
}

void Bank::deposit(unsigned int id, unsigned int amount) {
	accounts[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
	while (true) {
		if (amount <= accounts[id]) break;
		_Accept(deposit);
	}

	accounts[id] -= amount;
}
