#ifndef PLAYER_H
#define PLAYER_H

#include <uPRNG.h>

#include "q2printer.h"

_Coroutine Player {
	_Exception Schmilblick {};
	PRNG & prng;
	unsigned int id;
	Printer & printer;
	Player *left, *right;
	unsigned int deck;
	bool active, drinker;
	static unsigned int players_num;
	static void decrementPlayers();

	void main();

   public:
	enum { DEATH_DECK_DIVISOR = 7 };
	static void players(unsigned int num);
	Player(PRNG & prng, unsigned int id, Printer &printer);
	void start(Player & lp, Player & rp);
	void play(unsigned int deck);
	void drink();
};

#endif
