#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class game_logic {

public :

    int maxPlayers = 4;
    int playersInGame = 0;

	void updateGame() {
		//Call functions based to update the game
		genDeck();
		shuffleDeck();
		for(int i=0;i<=51;i++) {
            //cout << cardDeck[i] << endl;
		}
		shuffleDeck();
		for(int i=0;i<=51;i++) {
            //cout << cardDeck[i] << endl;
		}
	}

    string makeMove(int playerID, bool action){
        if (playerID == activePlayer) {
            if (action == 0) {
                stand(playerID);
            }
            else hit(playerID);
        }
        return "";
    }

    bool addPlayer(int playerID){
        if(playersInGame != maxPlayers){
                playersInGame++;
                vector<int>::iterator it;
                int ser = playerID;
                it = find(vec.begin(), vec.end(), ser);
                if (it != vec.end())
                {
                    cout << "Element " << ser << " found at position : ";
                    cout << it - vec.begin() + 1 << "\n";
                    vec.at(it - vec.begin() + 1) = playerID;
                    cout << "now " << vec.at(it - vec.begin() + 1);
                }
                else
                    cout << "Element not found.\n\n";
                }
    }
    bool removePlayer(int playerID){
        vector<int>::iterator it;
        int ser = playerID;
        it = find(vec.begin(), vec.end(), ser);
        if (it != vec.end())
        {
            cout << "Element " << ser << " found at position : ";
            cout << it - vec.begin() + 1 << "\n";
            vec.at(it - vec.begin() + 1) = 0;
            cout << "now " << vec.at(it - vec.begin() + 1);
        }
        else
            cout << "Element not found.\n\n";
    }

private :
    vector<int> vec{ 0, 0, 0, 0};
    int pHand[4];
    int dHand = 0;
    int cardCounter = 0;
	int activePlayer;//probably need to make a player class that can store some info
    int cardDeck[52];
    int deckSize;
    bool playerBust[4];
    bool dealerBust;

    void genDeck() {
        int b = 1;
        int j = 0;
        for(int i=0;i<=51;i++) {
            if(j == 4) {
                b++;
                j = 0;
            }
            if(b > 10) {
                b = 10;
            }
            cardDeck[i] = b;
            j++;
        }
    }

    void shuffleDeck() {
        random_shuffle(cardDeck, cardDeck + 52);
    }

    string hit(int playerID) {
        if (!playerBust[playerID] && !dealerBust) {
            pHand[playerID] = pHand[playerID] + cardDeck[cardCounter];
            cout << "Player now has: " << pHand[playerID] << endl;
            cardCounter++;
            return "Player now has" + pHand[playerID];
        }
        else if (dealerBust) {
            return "Disable hitting in this situation since the game is over";
        }

        if (pHand[playerID] > 21) {
            playerBust[playerID] = true;
            return "You can't hit anymore (use this shit to disable the hit option)";
        }
	}

	void stand(int playerID) {

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
        if (!dealerBust) {
            if (!houseStands()) {
                dHand = dHand + cardDeck[cardCounter];
                return "Dealer now has: " + dHand;
                cardCounter++;
            }
            else
                return "House can't draw anymore (This shit will be used to stop the dealer from drawing more once they reach 17)";
        }
        if (dHand > 21) {
            dealerBust = true;
            return "You can't hit anymore (use this shit to disable the hit option)";
        }
	}

	string compare(int playerID) {
	    if (!playerBust[playerID] && !dealerBust) {
            if (pHand[playerID] > dHand) {
                return "Player wins - player " + playerID;
            }
            else if (pHand[playerID] < dHand) {
                return "Dealer wins";
            }
            else if (pHand[playerID] == dHand) {
                return "It's a tie";
            }
        }
        else if (playerBust[playerID]) {
            return "Player busted - player " + playerID;
        }
        else if (dealerBust) {
            return "Dealer busted, player wins - player " + playerID;
        }
        reset(playerID);
	}

	void reset(int playerID) {
	    pHand[playerID] = 0;
        dHand = 0;
        playerBust[playerID] = 0;
        dealerBust = 0;
	}
};
