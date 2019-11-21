//Server Main
#include <iostream>
#include "game_logic.h"
#include "session_management.cpp"
#include <thread>
#include <string>

using namespace std;

ConnectionManager manager;
game_logic game; //State of the game

void ConnectionThread::operator()(SOCKET _ClientSocket) { //the function used by the threads
    ClientSocket = _ClientSocket;
    game.updateGame();
    game.addPlayer();
    playerConnected = true;
    string input="";
    string output="";
    if(playerConnected) {
        ClientMessage(game.deal());
        while(true) {
            input=ClientReceive();
            if (bytesReceived>0){
                printf("Input received\n");
                if(bytesReceived==3) {
                    string hitres = game.makeMove(true);
                    ClientMessage(hitres);
                    //printf("sent hit\n");
                } else {
                    string hitres = game.makeMove(false);
                    ClientMessage(hitres);
                    //printf("sent stand\n");
                }
                if(game.standing == true){
                   string hitres = game.doHouse();
                    ClientMessage(hitres);
                }
                input="";
                output="";
            } else {
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



