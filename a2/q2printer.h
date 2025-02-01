#ifndef PRINTER_H
#define PRINTER_H

class Printer {
	struct PImpl;  // hidden implementation
	PImpl* pimpl;

   public:
	Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards);
	~Printer();
	void print(unsigned int id, int took,
			   unsigned int RemainingPlayers);	// card play
	void print(unsigned int id);				// drink (Schmilblick)
};

#endif
