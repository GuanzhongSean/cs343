#ifndef __WATCARD_H__
#define __WATCARD_H__

#include <uFuture.h>

class WATCard {
	unsigned int balance;

	WATCard(const WATCard &) = delete;	// prevent copying
	WATCard(const WATCard &&) = delete;
	WATCard &operator=(const WATCard &) = delete;
	WATCard &operator=(const WATCard &&) = delete;

   public:
	typedef Future_ISM<WATCard *> FWATCard;	 // future watcard pointer
	WATCard();
	void deposit(unsigned int amount);
	void withdraw(unsigned int amount);
	unsigned int getBalance();
};

#endif
