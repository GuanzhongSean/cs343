#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <iostream>

using namespace std;

_Monitor Printer {
	int numInstances, numStudents, numVendingMachines, numCouriers;
	struct PrinterEntry {
		bool isSet;
		char state;
		unsigned int value1, value2;
		bool isValue1Set, isValue2Set;
		PrinterEntry();
		void print();
	};
	PrinterEntry *buffer;
	void flush();

   public:
	enum Kind {
		Parent,
		Groupoff,
		WATCardOffice,
		NameServer,
		Truck,
		BottlingPlant,
		Student,
		Vending,
		Courier
	};
	Printer(unsigned int numStudents, unsigned int numVendingMachines,
			unsigned int numCouriers);
	~Printer();
	void print(Kind kind, char state);
	void print(Kind kind, char state, unsigned int value1);
	void print(Kind kind, char state, unsigned int value1, unsigned int value2);
	void print(Kind kind, unsigned int lid, char state);
	void print(Kind kind, unsigned int lid, char state, unsigned int value1);
	void print(Kind kind, unsigned int lid, char state, unsigned int value1,
			   unsigned int value2);

   private:
	unsigned int printAndReturnIndex(Kind kind, unsigned int lid, char state);
};

#endif
