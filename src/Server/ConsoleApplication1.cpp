// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
using namespace std;
#include <iostream>
#include <vector>
#include "game_logic.cpp"

bool PlayerHit() {
	cout << "Do you wish to Hit or Stand?" << endl;
	string input;
	cin >> input;

	if (input == "Hit") {
		return true;
	}
	else {
		return false;
	}
}

int main()
{
	game_logic game;
	game.addPlayer(1);
	game.addPlayer(2);
	game.updateGame();
	game.deal(0);
	while (game.standing==true) {
		bool s = PlayerHit();
		game.makeMove(0, s);
	}
	return 0;
}

bool PlayerJoin() {
	cout << "Do you want to join a game??" << endl;
	cout << "[Y]es or [N]o" << endl;
	string input;
	cin >> input;

	if (input == "Y" || input == "y") {
		return true;
	}
	else {
		return false;
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
