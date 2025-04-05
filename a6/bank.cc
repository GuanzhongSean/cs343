#include "bank.h"

Bank::Bank(unsigned int numStudents) {
	accounts = new unsigned int[numStudents]();
}

Bank::~Bank() {
	delete[] accounts;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
	accounts[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
	while (amount > accounts[id]) {
		_Accept(deposit);
	}
	accounts[id] -= amount;
}
