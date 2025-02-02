#include <uPRNG.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "q1util.h"
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

	// Parse command line arguments
	if (argc > 4) {
		if (strcmp(argv[4], "d") != 0) {
			int seed;
			try {
				seed = convert(argv[4]);
			} catch (invalid_argument &) {
				usage(argv);
			}
			if (seed <= 0) {
				usage(argv);
			}
			prng.set_seed(seed);
			prng2.set_seed(seed);
		}
	}
	if (argc > 3) {
		if (strcmp(argv[3], "d") != 0) {
			try {
				cards = convert(argv[3]);
			} catch (invalid_argument &) {
				usage(argv);
			}
			if (cards <= 0) {
				usage(argv);
			}
		}
	}
	if (argc > 2) {
		if (strcmp(argv[2], "d") != 0) {
			try {
				players = convert(argv[2]);
			} catch (invalid_argument &) {
				usage(argv);
			}
			if (players < 2 || players > 20) {
				usage(argv);
			}
		}
	}
	if (argc > 1) {
		if (strcmp(argv[1], "d") != 0) {
			try {
				games = convert(argv[1]);
			} catch (invalid_argument &) {
				usage(argv);
			}
			if (games < 0) {
				usage(argv);
			}
		}
	}

	for (int g = 0; g < games; g++) {
		// if (g ==1) {
		// 	for (int i = 0; i < 10; i++) {
		// 		cout << prng(1, 8) << " ";
		// 	}
		// }
		if (g != 0) {
			cout << endl << endl;
		}
		if (argc <= 2) {
			players = prng2(2, 10);
		}
		if (argc <= 3) {
			cards = prng2(10, 200);
		}
		Player::players(players);
		Printer printer(players, cards);
		vector<Player *> playerList;
		for (int p = 0; p < players; p++) {
			playerList.push_back(new Player(prng, p, printer));
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
