#include <uPRNG.h>

#include <iostream>
#include <vector>

#include "q2player.h"
#include "q2printer.h"

int main(int argc, char *argv[]) {
	unsigned int games = 5;
	unsigned int players = 2;
	unsigned int cards = 10;
	unsigned int seed = 0;

	// Parse command line arguments
	if (argc > 1) games = std::stoi(argv[1]);
	if (argc > 2) players = std::stoi(argv[2]);
	if (argc > 3) cards = std::stoi(argv[3]);
	if (argc > 4) seed = std::stoi(argv[4]);

	PRNG prng(seed);
	Printer printer(players, cards);

	for (unsigned int g = 0; g < games; ++g) {
		std::vector<Player *> playerList;
		for (unsigned int p = 0; p < players; ++p) {
			playerList.push_back(new Player(prng, p, printer));
		}

		// Set up player circle
		for (unsigned int p = 0; p < players; ++p) {
			playerList[p]->start(*playerList[(p + players - 1) % players],
								 *playerList[(p + 1) % players]);
		}

		// Start game
		unsigned int starter = prng(players);
		playerList[starter]->play(cards);

		// Clean up
		for (auto p : playerList) {
			delete p;
		}
	}

	return 0;
}
