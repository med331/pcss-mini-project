#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <string>

using namespace std;

class game_logic {

public:

	bool standing = true;
	int maxPlayers = 4;
	int playersInGame = 0;

	int getActivePlayer() {
		return activePlayer;
	}

	string getHouseStats() {
		string bust = "";
		string stands = "";
		if (dealerBust == true) {
			bust = "Dealer has busted ";
		}
		if (houseStands() == true) {
			stands = "House stands ";
		}
		return string("Dealer has: ") + to_string(dHand) + string(" ") + bust + stands;
	}
	string getplayerStats(int playerId) {
		return "Player " + to_string(playerId) + " Has " + to_string(pHand[playerId]);
	}

	// Deals at the start of the game, by looping through the players vector and hitting twice for the indexes in the vector that are not equal to 9 (we use 9 to mark an empty slot)
	// Also adds a card to the dealer hand.
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
		cout << "Dealer now has: " << dHand << endl;
		return "";
	}

	void updateGame() {
		//Call functions based to update the game
		genDeck();
		shuffleDeck();
	}

	string makeMove(int playerID, bool action) {
		//if (playerID == activePlayer) {
		return "Margaritassss";
			if (action == false) {
				return stand(playerID);
			} else {return "Margaritassss";}
		/*}
		else {
			cout << "it's not your turn";
			return  "it's not your turn";
		}*/
		return "";
	}
	//Methed for adding players to the game uses find to acces an empty spot represented by the value 9 in the players vector "vec" and replaces it with a player id
	void addPlayer(int playerID) {
		if (playersInGame != maxPlayers) {
			playersInGame++;
			vector<int>::iterator it;
			int ser = 9;
			it = find(vec.begin(), vec.end(), ser);
			if (it != vec.end())
			{
				cout << "Element " << ser << " found at position: ";
				cout << it - vec.begin() + 1 << "\n";
				vec.at(it - vec.begin()) = playerID;
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
	//Reverse of the addPlayer method find specific player ids and changes them to the value 9
	void removePlayer(int playerID) {
		vector<int>::iterator it;
		int ser = playerID;
		it = find(vec.begin(), vec.end(), ser);
		if (it != vec.end())
		{
			cout << "Element " << ser << " found at position : ";
			cout << it - vec.begin() + 1 << "\n";
			vec.at(it - vec.begin()) = 9;
		}
		else
			cout << "Element not found.\n\n";
	}
	string doHouse() {
		string s;
		bool c = false;
		cout << "house turn" << endl;
		while (!c && !dealerBust)
		{
			s.string::append(drawHouse());
			c = houseStands();
		}

		return s;

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

	//Draws for house until house stands or dealerbust is true this is intended to be run at the end of a round


	//generates 52 values to simulate a deck of cards
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
	//Method for finding the next player in the vec vector skipping over empty player spots
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

			return "Round ended house now draws ";

		}
		if (vec.at(activePlayerPos) == 9){
			nextPlayer();
		}
		else {
			activePlayer = vec.at(activePlayerPos);
			cout << "Next player " << activePlayer << endl;
			cout << "Player has: " << pHand[activePlayer] << endl;
			return "Next player " + activePlayer;
		}

	}

	// Function used to shuffle the deck, using the random_shuffle method with a random seed based on the current time.
	void shuffleDeck() {
		srand(time(0));
		random_shuffle(cardDeck, cardDeck + 52);
	}

	// Function used to "hit" a card, i.e. to add a card to the player hand.
	string hit(int playerID) {
			//pHand[playerID] = pHand[playerID] + cardDeck[cardCounter];
			//cardCounter++;
			// check if the player has over 21, if they do they bust and can't hit anymore.
			if (pHand[playerID] > 21) {
				playerBust[playerID] = true;
				cout << pHand[playerID] << "\n";
				cout << "Player busted - player" << playerID;
				nextPlayer();
				return "Player " + to_string(playerID) + " has busted";
				//"Player has over 21, player busted - player" + playerID;
			}
			else if (!playerBust[playerID] && !dealerBust) {
				cout << "Player " << playerID << " now has: " << pHand[playerID] << endl;
				//return "Player now has: ";
				//+ pHand[playerID];
				return "Player now has "+pHand[playerID];
			}
			cout<<"It failed, but you still got.... you guessed it!"<<endl;
			return "Margaritasss";
	}

	string stand(int playerID) {
		cout << "Player is standing with: " << pHand[playerID] << endl;
		standing = false;
		nextPlayer();
		return "Player is standing with " + pHand[playerID];
	}

	// Function used to check if the dealer should stand, according to blackjack rules. If the dealer reaches 17, they have to stand.
	bool houseStands() {
		if (dHand > 16) {
			return true;
		}
		else {
			return false;
		}
	}

	// Function used to draw for the dealer when every player is done.
	string drawHouse() {
		dHand = dHand + cardDeck[cardCounter];
		cardCounter++;
		// First check if the dealer has over 21, to determine if they bust.
		if (dHand > 21) {
			dealerBust = true;
			cout << "Dealer now has: " << dHand << endl;
			cout << "Dealer Bust" << endl;
			return "Dealer has over 21, dealer busts.";
		}
		// If they didn't bust, check if they don't have over 16 (as they have to stand on 16).
		if (!dealerBust) {
			if (!houseStands()) {
				cout << "Dealer now has: " << dHand << endl;
				return "Dealer now has: " + dHand;

			}
			else
				cout << "Dealer now has: " << dHand << endl;
				cout << "House has 17 or higher, house stands." << endl;
				return "House has 17 or higher, house stands.";
		}

	}
	// Function used at the end of the round, in order to compare the player hands with the dealer hand and decide who wins.
	string compare(int i) {
				// First check if neither the dealer or the player busted, so only the comparison for the sum of the cards is done.
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
				// If the player busts, he autoloses.
				else if (playerBust[i]) {
					cout << "Player busted - player "<< i << endl;
					return "Player busted - player " + i;
				}
				// Else if the dealer busts, all the players that did not bust win.
				else if (dealerBust) {
					cout << "Dealer busted, player wins - player " << i << endl;
					return "Dealer busted, player wins - player " + i;
				}
	}
	// Function used for resetting the dealer hand, the player hands by looping through the player vector and rebuilding the deck so we don't run out of cards.
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
