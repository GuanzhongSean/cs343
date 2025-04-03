#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <iostream>

using namespace std;

_Monitor Printer {
	int numInstances;		 // total number of columns (instances)
	int numStudents;		 // number of student instances
	int numVendingMachines;	 // number of vending machine instances
	int numCouriers;		 // number of courier instances

	struct Info {			  // store the information of an instance
		bool isSet;			  // true if currently store valid information
		char state;			  // the state of the instance
		unsigned int value1;  // value1 of the instance
		bool isValue1Set;	  // true if value1 is set
		unsigned int value2;  // value2 of the instance
		bool isValue2Set;	  // true if value2 is set
		Info();				  // constructor
		void printInfo();	  // print the information
	};

	Info *buffer;  // store information for each kind of instance
	void flush();  // flush all the stored information
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
	// same as print( Kind kind, unsigned int lid, char state ), but return the index of
	// the instance in the buffer
	int printAndReturnIndex(Kind kind, unsigned int lid, char state);
};

#endif
