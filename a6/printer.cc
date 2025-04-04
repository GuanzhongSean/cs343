#include "printer.h"

Printer::Info::Info()
	: isSet(false),
	  state(0),
	  value1(-1),
	  isValue1Set(false),
	  value2(-1),
	  isValue2Set(false) {}

void Printer::Info::printInfo() {
	cout << state;	// print state
	if (isValue1Set) {
		cout << value1;
	}
	if (isValue2Set) {
		cout << "," << value2;
	}
}

void Printer::flush() {
	// calculate the last column with stored information
	int lastCol = 0;
	for (int i = numInstances - 1; i >= 0; i -= 1) {
		if (buffer[i].isSet) {	// found and last column with information, then break
			lastCol = i;
			break;
		}
	}

	// print all the stored information
	for (int i = 0; i < numInstances; i++) {
		if (buffer[i].isSet) {
			buffer[i].printInfo();
			buffer[i].isSet = false;
			buffer[i].isValue1Set = false;
			buffer[i].isValue2Set = false;
		}

		if (i < lastCol) {	// don't print tab after the last stored information
			cout << "\t";
		}
	}

	cout << endl;
}

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines,
				 unsigned int numCouriers)
	: numStudents(numStudents),
	  numVendingMachines(numVendingMachines),
	  numCouriers(numCouriers) {
	// calcu total number of columns, 6 includes parent, groupoff, WATCard office, name
	// server, truck, bottling plant
	numInstances = 6 + numStudents + numVendingMachines + numCouriers;
	cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant\t";

	for (unsigned int i = 0; i < numStudents; i++) {
		cout << "Stud" << i << "\t";
	}

	for (unsigned int i = 0; i < numVendingMachines; i++) {
		cout << "Mach" << i << "\t";
	}

	for (unsigned int i = 0; i < numCouriers; i++) {
		cout << "Cour" << i;
		if (i != numCouriers - 1) {
			cout << "\t";
		}
	}

	cout << endl;

	for (int i = 0; i < numInstances; i++) {  // print second line
		cout << "*******";
		if (i != numInstances - 1) {  // don't print tab at the end of line
			cout << "\t";
		}
	}
	cout << endl;

	buffer = new Info[numInstances];  // initiazlie the buffer
}

Printer::~Printer() {
	flush();									// flush all the stored information
	delete[] buffer;							// free memory
	cout << "***********************" << endl;	// more output at the end
}

void Printer::print(Kind kind, char state) {
	// flush the buffered data if the column is about to be overwritten
	if (buffer[kind].isSet) {
		flush();
	}

	// store the information
	buffer[kind].isSet = true;
	buffer[kind].state = state;
}

void Printer::print(Kind kind, char state, unsigned int value1) {
	print(kind, state);
	buffer[kind].value1 = value1;
	buffer[kind].isValue1Set = true;
}

void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2) {
	print(kind, state, value1);
	buffer[kind].value2 = value2;
	buffer[kind].isValue2Set = true;
}

int Printer::printAndReturnIndex(Kind kind, unsigned int lid, char state) {
	// calcu the index of instance in the buffer
	int i = 6;
	switch (kind) {
		case Kind::Student:
			i += lid;
			break;
		case Kind::Vending:
			i += numStudents + lid;
			break;
		case Kind::Courier:
			i += numStudents + numVendingMachines + lid;
			break;
		default:  // invalid kind
			cerr << "passed invalid kind" << endl;
			break;
	}

	// flush the buffered data if the column is about to be overwritten
	if (buffer[i].isSet) {
		flush();
	}

	// store the information
	buffer[i].isSet = true;
	buffer[i].state = state;
	return i;
}

void Printer::print(Kind kind, unsigned int lid, char state) {
	printAndReturnIndex(kind, lid, state);
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1) {
	int i = printAndReturnIndex(kind, lid, state);
	buffer[i].value1 = value1;
	buffer[i].isValue1Set = true;
}

void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1,
					unsigned int value2) {
	int i = printAndReturnIndex(kind, lid, state);
	buffer[i].value1 = value1;
	buffer[i].isValue1Set = true;
	buffer[i].value2 = value2;
	buffer[i].isValue2Set = true;
}
