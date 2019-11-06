#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class game_logic {

public :

    int maxPlayers = 4;
    int playersInGame = 0;

	void updateGame(){
		//Call functions based to update the game
		genDeck();
		shuffleDeck();
		for(int i=0;i<=51;i++){
            //cout << cardDeck[i] << endl;
		}
		shuffleDeck();
		for(int i=0;i<=51;i++){
            //cout << cardDeck[i] << endl;
		}

    bool makeMove(int playerID, bool action){
        return false;
    }

    bool addPlayer(int playerID){
        return false;
    }

    bool removePlayer(int playerID){
        return false;
    }
private :

    int pHand = 0;
    int dHand = 0;
    int cardCounter = 0;
	int activePlayer;//probably need to make a player class that can store some info
    int cardDeck[52];
    int deckSize;
    bool playerBust;
    bool dealerBust;
    void genDeck() {
        int b = 1;
        int j = 0;
        for(int i=0;i<=51;i++){
            if(j == 4){
                b++;
                j = 0;
            }
            if(b > 10){
                b = 10;
            }
            cardDeck[i] = b;
            j++;
        }
    }

    void shuffleDeck() {
        std::random_shuffle(cardDeck, cardDeck + 52);
    }

	void deal() {

	}

    void hit() {
        if (!playerBust && !dealerBust) {
            pHand = pHand + cardDeck[cardCounter];
            cout << "Player now has: " << pHand << endl;
            cardCounter++;
        }
        else if (dealerBust){
            cout << "Disable hitting in this situation since the game is over" << endl;
        }

        if (pHand > 21){
            playerBust = true;
            cout << "You can't hit anymore (use this shit to disable the hit option)" << endl;
        }
	}

	void stand() {

	}

	void getHand() {

	}

	bool houseStands() {
        if (dHand > 16) {
            return true;
        }
        else {
            return false;
        }
	}

	void drawHouse() {
        if (!dealerBust) {
            if (!houseStands()){
                dHand = dHand + cardDeck[cardCounter];
                cout << "Dealer now has: " << dHand << endl;
                cardCounter++;
            }
            else
                cout << "House can't draw anymore (This shit will be used to stop the dealer from drawing more once they reach 17)" << endl;
        }
        if (dHand > 21){
            dealerBust = true;
            cout << "You can't hit anymore (use this shit to disable the hit option)" << endl;
        }
	}

	void compare() {
	    if (!playerBust && !dealerBust){
            if (pHand > dHand){
                cout << "Player wins" << endl;
            }
            else if (pHand < dHand){
                cout << "Dealer wins" << endl;
            }
            else if (pHand = dHand){
                cout << "It's a tie" << endl;
            }
        }
        else if (playerBust){
            cout << "Player busted" << endl;

        }
        else if (dealerBust){
            cout << "Dealer busted, player wins" << endl;
        }
        reset();
	}

	void reset() {
	    pHand = 0;
        dHand = 0;
        playerBust = 0;
        dealerBust = 0;
	}

	void bust_check(int hand){

	}

};
