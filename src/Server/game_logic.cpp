#include <iostream>

using namespace std;

class game_logic {

public :

	void updateGame(){
		//Call functions based to update the game
		genDeck();
		for(int i=0;i<=51;i++){
            cout << cardDeck[i];
		}
	}


private :


	int activePlayer;//probably need to make a player class that can store some info
	enum Value { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
	enum Suit {HEARTS,DIAMONDS,SPADES,CLUBS};
    int cardDeck[52];
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

	void deal() {

	}

    void hit() {

	}

	void stand() {

	}

	void getHand() {

	}

	void checkHouse() {
        //check house card decide if house hits or stands
	}

	void drawHouse() {

	}

	void compare() {

	}

};
