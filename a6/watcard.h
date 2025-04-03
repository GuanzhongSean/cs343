#ifndef __WATCARD_H__
#define __WATCARD_H__

#include <uFuture.h>

class WATCard {
	unsigned int balance;  // the balance of the watcard

	WATCard(const WATCard &) = delete;	// prevent copying
	WATCard(const WATCard &&) = delete;
	WATCard &operator=(const WATCard &) = delete;
	WATCard &operator=(const WATCard &&) = delete;

   public:
	typedef Future_ISM<WATCard *> FWATCard;	 // future watcard pointer
	WATCard();								 // constructor
	void deposit(unsigned int amount);		 // add money to balance
	void withdraw(unsigned int amount);		 // move money from balance
	unsigned int getBalance();				 // return balance
};

#endif
