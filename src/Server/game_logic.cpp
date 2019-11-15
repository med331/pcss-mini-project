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

	int getActivePlayer() {
		return activePlayer;
	}

	//update needs to deal only to spots that are filled
	string deal() {
		for (int i = 0; i <=3; i++) {
			if (vec.at(i) != 9) {
				hit(i);
				hit(i);
			}
			else {
				cout << "skipped" << i;
			}
		}
		dHand = 0 + cardDeck[cardCounter];
		cardCounter++;
		cout << "Dealer now has:: " << dHand << endl;
		return "";
	}
	
	void updateGame() {
		//Call functions based to update the game
		genDeck();
		shuffleDeck();
	}

	string makeMove(int playerID, bool action) {
		if (playerID == activePlayer) {
			if (action == false) {
				return stand(playerID);
			}
			else return hit(playerID);
		}
		else {
			cout << "it's not your turn";
			return  "it's not your turn";
		}
		return "";
	}

	void addPlayer(int playerID) {
		if (playersInGame != maxPlayers) {
			playersInGame++;
			vector<int>::iterator it;
			int ser = 9;
			it = find(vec.begin(), vec.end(), ser);
			if (it != vec.end())
			{
				cout << "Element " << ser << " found at position : ";
				cout << it - vec.begin() + 1 << "\n";
				vec.at(it - vec.begin()) = playerID;
				cout << "now " << vec.at(it - vec.begin());
				if (vec.at(it - vec.begin()) == 0) {
					activePlayer = vec.at(it - vec.begin());
					cout << "activePlayer is now " << activePlayer << endl;
					activePlayerPos = 0;
				}
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
			vec.at(it - vec.begin()) = 9;
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

	}

private:
	vector<int> vec{ 9, 9, 9, 9 };
	int pHand[4];
	int dHand = 0;
	int cardCounter = 0;
	int activePlayer;
	int activePlayerPos;
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
	//update needs to run in a way where it only selects filled spots as active player and ignores empty spots
	string nextPlayer() {
		activePlayerPos++;
		cout << "CURRENT PLAYER: " << activePlayerPos << endl;
		if (activePlayerPos > 3) {
			activePlayerPos = 0;
			activePlayer = vec.at(activePlayerPos);
			cout << "round finished house turn " << endl;
			doHouse();
			for (int i = 0; i < 4; i++) {
				if (vec.at(i) != 9) {
					compare(i);
				}
			}
			cout << "New round starting... " << endl;
			reset();
			deal();
			
			return "Next player ";
				
		}
		if (vec.at(activePlayerPos) == 9){
			nextPlayer();
		}
		else {
			activePlayer = vec.at(activePlayerPos);
			cout << "Next player " << activePlayer << endl;
			cout << "Player has: " << pHand[activePlayer] << endl;
			return "Next player";
		}		
		
	}

	void shuffleDeck() {
		srand(time(0));
		random_shuffle(cardDeck, cardDeck + 52);
	}

	string hit(int playerID) {
			pHand[playerID] = pHand[playerID] + cardDeck[cardCounter];
			cardCounter++;
			if (pHand[playerID] > 21) {
				playerBust[playerID] = true;
				cout << pHand[playerID] << "\n";
				cout << "Player busted - player" << playerID;
				stand(playerID);
				return "You can't hit anymore (use this shit to disable the hit option)";
			}
			else if (!playerBust[playerID] && !dealerBust) {
				cout << "Player " << playerID << "now has: " << pHand[playerID] << endl;
				return "Player now has: " + pHand[playerID];
			}
	}

	string stand(int playerID) {
		cout << "Player is standing with: " << pHand[playerID] << endl;
		standing = false;
		nextPlayer();
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
			cout << "Dealer Bust" << endl;
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
	//update?
	string compare(int i) {
				if (!playerBust[i] && !dealerBust) {
					if (pHand[i] > dHand) {
						cout << "Player wins - player "<< i << endl;
						return "Player wins - player " + i;
					}
					else if (pHand[i] < dHand) {
						cout << "Dealer wins" << endl;
						return "Dealer wins against player " + i;
					}
					else if (pHand[i] == dHand) {
						cout << "It's a tie " << endl;
						return "It's a tie between dealer and player " + i;
					}
				}
				else if (playerBust[i]) {
					cout << "Player busted - player "<< i << endl;
					return "Player busted - player " + i;
				}
				else if (dealerBust) {
					cout << "Dealer busted, player wins - player " << i << endl;
					return "Dealer busted, player wins - player " + i;
				}
	}
		
	void reset() {
		for (int i = 0; i < 4; i++) {
			if (vec.at(i) != 9) {
				pHand[i] = 0;
				playerBust[i] = 0;
			}
		}
		dHand = 0;
		dealerBust = 0;
		genDeck();
		shuffleDeck();
		cardCounter = 0;
	}
};
