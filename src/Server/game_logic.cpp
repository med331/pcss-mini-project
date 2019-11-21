#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <string>

using namespace std;

class game_logic {

public:

	bool standing = false;
	int maxPlayers = 1;
	int playersInGame = 0;

	/*int getMaxPlayer() {
		return maxPlayers;
	}*/

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
	string getplayerStats() {
		return "Player Has " + to_string(pHand);
	}

	// Deals at the start of the game, by looping through the players vector and hitting twice for the indexes in the vector that are not equal to 9 (we use 9 to mark an empty slot)
	// Also adds a card to the dealer hand.
	string deal() {

        hit();
        hit();

		drawHouse();

		return "Dealing out cards \n" + getplayerStats() + "\n" + "Dealer now has: " + to_string(dHand) + "\n";
	}

	void updateGame() {
		//Call functions based to update the game
		genDeck();
		shuffleDeck();
	}

	string makeMove(bool action) {
			if (action == false) {
				return stand();
			} else {return hit();}
		return "Making a move failed";
	}
	//Method for adding players to the game uses find to acces an empty spot represented by the value 9 in the players vector "vec" and replaces it with a player id
	void addPlayer() {
		if (playersInGame != maxPlayers) {
			playersInGame++;
			vector<int>::iterator it;
			int ser = 9;
			it = find(vec.begin(), vec.end(), ser);
			if (it != vec.end())
			{
				vec.at(it - vec.begin()) = 0;
				if (vec.at(it - vec.begin()) == 0) {
					activePlayer = vec.at(it - vec.begin());
					activePlayerPos = 0;

				}
			}
			else
				cout << "Element not found.\n\n";
		}
	}

	//Reverse of the addPlayer method find specific player ids and changes them to the value 9
	void removePlayer() {
		vector<int>::iterator it;
		int ser = 0;
		it = find(vec.begin(), vec.end(), ser);
		if (it != vec.end())
		{
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

		return s + compare(0);

	}
private:
	vector<int> vec{ 9};
	int pHand;
	int dHand = 0;
	int cardCounter = 0;
	int activePlayer;
	int activePlayerPos;
	int cardDeck[52];
	int deckSize;
	bool playerBust;
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

	// Function used to shuffle the deck, using the random_shuffle method with a random seed based on the current time.
	void shuffleDeck() {
		srand(time(0));
		random_shuffle(cardDeck, cardDeck + 52);
	}

	// Function used to "hit" a card, i.e. to add a card to the player hand.
	string hit() {
			pHand = pHand + cardDeck[cardCounter];
			cardCounter++;
			// check if the player has over 21, if they do they bust and can't hit anymore.
			if (pHand > 21) {
				playerBust = true;
				cout << pHand << "\n";
				cout << "Player busted"<<endl;
				standing = true;
				return "Player has over 21, player busted" + doHouse() + "\n";
			}
			else if (!playerBust && !dealerBust) {
				cout << "Player now has: " << pHand << endl;
				return "Player now has "+to_string(pHand) + "\n";
			} else {return "Hit failed";}
	}

	string stand() {
		cout << "Player is standing with: " << pHand << endl;
		standing = true;
		return "Player is standing with " + to_string(pHand) + "\n";
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
			return "Dealer has over 21, dealer busts. \n";
		}
		// If they didn't bust, check if they don't have over 16 (as they have to stand on 16).
		if (!dealerBust) {
			if (!houseStands()) {
				cout << "Dealer now has: " << dHand << endl;
				return "Dealer now has: " + to_string(dHand) + "\n";

			}
			else
				cout << "Dealer now has: " << dHand << endl;
				cout << "House has 17 or higher, house stands." << endl;
				return "Dealer has "+ to_string(dHand) +", house stands. \n";
		}

	}
	// Function used at the end of the round, in order to compare the player hands with the dealer hand and decide who wins.
	string compare(int i) {
				// First check if neither the dealer or the player busted, so only the comparison for the sum of the cards is done.
				if (!playerBust && !dealerBust) {
					if (pHand > dHand) {
						cout << "Player wins - player "<< i << endl;
						return "Player wins - player " + to_string(i) + "\n" + reset();
					}
					else if (pHand < dHand) {
						cout << "Dealer wins" << endl;
						return "Dealer wins against player " + to_string(i)+ "\n" + reset();
					}
					else if (pHand == dHand) {
						cout << "It's a tie " << endl;
						return "It's a tie between dealer and player " + to_string(i)+ "\n" + reset();
					}
				}
				// If the player busts, he autoloses.
				else if (playerBust) {
					cout << "Player busted - player "<< i << endl;
					return "Player busted - player " + to_string(i)+ "\n" + reset();
				}
				// Else if the dealer busts, all the players that did not bust win.
				else if (dealerBust) {
					cout << "Dealer busted, player wins - player " << i << endl;
					return "Dealer busted, player wins - player " + to_string(i)+ "\n" + reset();
				}
	}
	// Function used for resetting the dealer hand, the player hands by looping through the player vector and rebuilding the deck so we don't run out of cards.
    string reset() {
        if (vec.at(0) != 9) {
            pHand = 0;
            playerBust = 0;
        }
        dHand = 0;
        dealerBust = 0;
        genDeck();
        standing = false;
        shuffleDeck();
        cardCounter = 0;
        return deal();
	}
};
