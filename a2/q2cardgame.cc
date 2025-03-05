#include <uPRNG.h>

#include <cstring>
#include <iostream>

#include "q2player.h"
#include "q2printer.h"

using namespace std;

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0]
		 << " [ games (>=0) | 'd' (default 5) [ players (>=2 && <=20) | "
			"'d' (random 2-10) [ cards (>0) | 'd' (random 10-200) [ seed "
			"(>0) | 'd' (random) ] ] ] ]"
		 << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	PRNG prng, prng2;
	int games = 5, players, cards;
	struct cmd_error {};

	// Parse command line arguments
	try {
		switch (argc) {
			case 5:
				if (strcmp(argv[4], "d") != 0) {
					int seed = convert(argv[4]);
					if (seed <= 0) {
						throw cmd_error{};
					}
					prng.set_seed(seed);
					prng2.set_seed(seed);
				}
			case 4:
				if (strcmp(argv[3], "d") != 0) {
					cards = convert(argv[3]);
					if (cards <= 0) {
						throw cmd_error{};
					}
				}
			case 3:
				if (strcmp(argv[2], "d") != 0) {
					players = convert(argv[2]);
					if (players < 2 || players > 20) {
						throw cmd_error{};
					}
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {
					games = convert(argv[1]);
					if (games < 0) {
						throw cmd_error{};
					}
				}
			case 1:
				break;
			default:
				throw cmd_error{};
		}
	} catch (...) {
		usage(argv);
	}

	for (int g = 0; g < games; g++) {
		if (g != 0) {
			cout << endl << endl;
		}
		if (argc <= 2 || strcmp(argv[2], "d") == 0) {
			players = prng2(2, 10);
		}
		if (argc <= 3 || strcmp(argv[3], "d") == 0) {
			cards = prng2(10, 200);
		}
		Player::players(players);
		Printer printer(players, cards);
		Player *playerList[players];
		for (int p = 0; p < players; p++) {
			playerList[p] = new Player(prng, p, printer);
		}

		// Set up player circle
		for (int p = 0; p < players; p++) {
			playerList[p]->start(*playerList[(p + players - 1) % players],
								 *playerList[(p + 1) % players]);
		}

		// Start game
		int starter = prng2(players);
		playerList[starter]->play(cards);

		// Clean up
		for (Player *p : playerList) {
			delete p;
		}
	}

	return 0;
}
