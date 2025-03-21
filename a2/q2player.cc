#include "q2player.h"

unsigned int Player::players_num;

void Player::players(unsigned int num) {
	players_num = num;
}

void Player::decrementPlayers() {
	players_num--;
}

Player::Player(PRNG &prng, unsigned int id, Printer &printer)
	: prng(prng),
	  id(id),
	  printer(printer),
	  left(nullptr),
	  right(nullptr),
	  active(true),
	  drinker(false) {}

void Player::start(Player &lp, Player &rp) {
	left = &lp;
	right = &rp;
	resume();
}

void Player::play(unsigned int deck) {
	this->deck = deck;
	resume();
}

void Player::drink() {
	resume();
}

void Player::main() {
	suspend();
	try {
		while (true) {
			_Enable;
			if (!active) {	// Check if player is still active
				if (&resumer() == left) {
					right->play(deck);
				} else {
					left->play(deck);
				}
				continue;
			}

			// Check if it's the last player
			if (players_num <= 1) {
				printer.print(id, deck, players_num);
				break;
			}

			// Take cards
			unsigned int take = prng(1, 8);
			if (take > deck) take = deck;
			deck -= take;
			printer.print(id, take, players_num);
			if (deck == 0) {
				break;
			}

			// Check for death deck
			if ((deck + take) % DEATH_DECK_DIVISOR == 0) {
				active = false;
				decrementPlayers();
			}

			// Schmilblick event
			if (active && prng(10) == 0) {
				_Resume Schmilblick();
			}

			// Pass deck to next player
			if (deck % 2 == 0) {
				right->play(deck);
			} else {
				left->play(deck);
			}
		}
	}
	_CatchResume((*left).Schmilblick &) {  // Schmilblick event propagate
		if (!drinker) {
			if (active) printer.print(id);
			_Resume Schmilblick() _At *right;
			right->drink();
		} else {
			drinker = false;
		}
	}
	_CatchResume((*this).Schmilblick &) {  // Schmilblick event start
		drinker = true;
		printer.print(id);
		_Resume Schmilblick() _At *right;
		right->drink();
	}
}
