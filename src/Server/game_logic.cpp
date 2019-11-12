#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <time.h>

using namespace std;

class game_logic {

public:
	bool standing = true;
	int maxPlayers = 4;
	int playersInGame = 0;

	string deal(int playerID) {
		hit(playerID);
		hit(playerID);
		drawHouse();
		return "";
	}

	void updateGame() {
		//Call functions based to update the game
		genDeck();
			shuffleDeck();
					for (int i = 0; i <= 51; i++) {
						//cout << cardDeck[i] << endl;
					}
					shuffleDeck();
					for (int i = 0; i <= 51; i++) {
						//cout << cardDeck[i] << endl;
					}
		
	}

	string makeMove(int playerID, bool action) {
		if (playerID == activePlayer) {
			if (action == false) {
				return stand(playerID);
			}
			else return hit(playerID);
		}
		return "";
	}

	void addPlayer(int playerID) {
		if (playersInGame != maxPlayers) {
			playersInGame++;
			vector<int>::iterator it;
			int ser = 0;
			it = find(vec.begin(), vec.end(), ser);
			if (it != vec.end())
			{
				cout << "Element " << ser << " found at position : ";
				cout << it - vec.begin() + 1 << "\n";
				vec.at(it - vec.begin()) = playerID;
				cout << "now " << vec.at(it - vec.begin());
			}
			else
				cout << "Element not found.\n\n";
		}
	}
	void removePlayer(int playerID) {
		vector<int>::iterator it;
		int ser = playerID;
		it = find(vec.begin(), vec.end(), ser);
		if (it != vec.end())
		{
			cout << "Element " << ser << " found at position : ";
			cout << it - vec.begin() + 1 << "\n";
			vec.at(it - vec.begin()) = 0;
			cout << "now " << vec.at(it - vec.begin());
		}
		else
			cout << "Element not found.\n\n";
	}
	void doHouse() {
		bool c = false;
		cout << "house turn" << endl;
		while (!c && !dealerBust)
		{
			drawHouse();
			c = houseStands();
		}
		compare();
	}

private:
	vector<int> vec{ 0, 0, 0, 0 };
	int pHand[4];
	int dHand = 0;
	int cardCounter = 0;
	int activePlayer = vec.at(0);//probably need to make a player class that can store some info
	int cardDeck[52];
	int deckSize;
	bool playerBust[4];
	bool dealerBust = false;

	void genDeck() {
		int b = 1;
		int j = 0;
		for (int i = 0; i <= 51; i++) {
			if (j == 4) {
				b++;
				j = 0;
			}
			if (b > 10) {
				b = 10;
			}
			cardDeck[i] = b;
			j++;
		}
	}

	void nextPlayer() {
		if (activePlayer == 0) {
			for (int i = 0; activePlayer > 0; i++) {
				activePlayer = vec.at(i);
			}
		}
		else if (activePlayer > 4) {
			compare();
		}
		else {
			activePlayer++;
		}
	}

	void shuffleDeck() {
		srand(time(0));
		random_shuffle(cardDeck, cardDeck + 52);
	}

	string hit(int playerID) {
		cout << playerBust[playerID];
		pHand[playerID] = pHand[playerID] + cardDeck[cardCounter];
		cardCounter++;
		cout << pHand[playerID] << "\n";
		cout << playerBust[playerID] << "\n";

		if (pHand[playerID] > 21) {
			playerBust[playerID] = true;
			cout << pHand[playerID] << "\n";
			cout << "You can't hit anymore (use this shit to disable the hit option)";
			stand(playerID);
			return "You can't hit anymore (use this shit to disable the hit option)";
		}
		else if (!playerBust[playerID] && !dealerBust) {
			cout << "Player now has: " << pHand[playerID] << endl;
			return "Player now has" + pHand[playerID];
		}
		else if (dealerBust) {
			return "Disable hitting in this situation since the game is over";
		}
	}

	string stand(int playerID) {
		cout << "Player is standing with " << pHand[playerID];
		standing = false;
		doHouse();
		return "Player is standing with " + pHand[playerID];
	}

	bool houseStands() {
		if (dHand > 16) {
			return true;
		}
		else {
			return false;
		}
	}

	string drawHouse() {
		dHand = dHand + cardDeck[cardCounter];
		cardCounter++;
		if (dHand > 21) {
			dealerBust = true;
			cout << "Dealer now has: " << dHand << endl;
			cout << "You can't hit anymore (use this shit to disable the hit option)" << endl;
			return "You can't hit anymore (use this shit to disable the hit option)";
		}
		if (!dealerBust) {
			if (!houseStands()) {
				cout << "Dealer now has: " << dHand << endl;
				return "Dealer now has: " + dHand;
				
			}
			else
				cout << "Dealer now has: " << dHand << endl;
				cout << "House can't draw anymore (This shit will be used to stop the dealer from drawing more once they reach 17)" << endl;
				return "House can't draw anymore (This shit will be used to stop the dealer from drawing more once they reach 17)";
		}
		
	}

	string compare() {
		for (int i = 0; i < 4; i++) {
			if (vec.at(i) != 0) {
				if (!playerBust[i] && !dealerBust) {
					if (pHand[i] > dHand) {
						cout << "Player wins - player " << endl;
						return "Player wins - player " + i;
					}
					else if (pHand[i] < dHand) {
						cout << "Dealer wins" << endl;
						return "Dealer wins";
					}
					else if (pHand[i] == dHand) {
						cout << "It's a tie " << endl;
						return "It's a tie";
					}
				}
				else if (playerBust[i]) {
					cout << "Player busted - player " << endl;
					return "Player busted - player " + i;
				}
				else if (dealerBust) {
					cout << "Dealer busted, player wins - player " << endl;
					return "Dealer busted, player wins - player " + i;
				}
			}
			reset();
		}
	}

	void reset() {
		for (int i = 0; i < 3; i++) {
			pHand[i] = 0;
			playerBust[i] = 0;
		}
		dHand = 0;
		dealerBust = 0;
	}
};
