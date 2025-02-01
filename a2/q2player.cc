#include <iostream>

#include "q2player.h"

void Player::players(unsigned int num) {
	// Static method to set the number of players
}

Player::Player(PRNG &prng, unsigned int id, Printer &printer)
	: prng(prng),
	  id(id),
	  printer(printer),
	  left(nullptr),
	  right(nullptr),
	  active(true) {}

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
    for (;;) {
        if (!active) break;

        // Take cards
        unsigned int take = prng(1, 8);
        if (take > deck) take = deck;
        deck -= take;

        // Check for death deck
        if (deck % DEATH_DECK_DIVISOR == 0) {
            active = false;
            printer.print(id, take, 0); // 0 indicates termination (X)
        } else {
            // Pass deck to next player
            if (deck % 2 == 0) {
                right->play(deck);
                printer.print(id, take, deck); // Positive value indicates pass to the right (>)
            } else {
                left->play(deck);
                printer.print(id, take, -deck); // Negative value indicates pass to the left (<)
            }
        }

        // Schmilblick event
        if (prng(10) == 0) {
            drink();
        }
    }
}
