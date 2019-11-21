//Server Main
#include <iostream>
#include "game_logic.h"
#include "session_management.cpp"
#include <thread>
#include <string>

using namespace std;

ConnectionManager manager;
game_logic game; //State of the game

void ConnectionThread::operator()(SOCKET _ClientSocket) { //the function used by the thread
    ClientSocket = _ClientSocket;
    game.updateGame(); // Sets up game
    game.addPlayer();  // Adds the player
    playerConnected = true;
    string input="";
    string output="";
    if(playerConnected) {
        ClientMessage(game.deal()); // Sends initial set of dealt hands
        while(true) {
            input=ClientReceive();
            if (bytesReceived>0){
                printf("Input received\n");
                if(bytesReceived==3) { // If hit
                    string hitres = game.makeMove(true); // Then hit
                    ClientMessage(hitres);

                } else {
                    string hitres = game.makeMove(false); // Else stand
                    ClientMessage(hitres);

                }
                if(game.standing == true){  // If player is standing
                   string hitres = game.doHouse(); // Then the house goes
                    ClientMessage(hitres);
                }
                input="";
                output="";
            } else { // If no bytes received, then remove player
                printf("Player disconnected");
                manager.Unsubscribe(ClientSocket);
                ClientSocket=-1;
                game.removePlayer();
                break;
            }
        }
    }
}

int main()
{
    manager.ServerSetup();
    return 0;
}



