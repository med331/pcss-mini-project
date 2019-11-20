//Server Main
#include <iostream>
#include "game_logic.h"
#include "session_management.cpp"
#include <thread>
#include <string>

using namespace std;

ConnectionManager manager;

void ConnectionThread::operator()(SOCKET _ClientSocket) { //the function used by the threads
    ClientSocket = _ClientSocket;
    playerConnected = true;
    string input="";
    string output="";
    //recv(ClientSocket, NULL, DEFAULT_BUFLEN, 0);
    if(playerConnected) {
        printf("connected\n");
        if(playerID==manager.GetGameLogic().getActivePlayer()) {
            printf("active player\n");
            //if(ClientMessage("your turn")>0) {
                //printf("sent\n");
                while(true) {
                    input=ClientReceive();
                    printf("received\n");
                    cout<<"bytes: "<<bytesReceived<<endl;
                    cout<<"input: "<<input<<endl;
                    if(bytesReceived==3) {
                        ClientMessage("Hit");//manager.GetGameLogic().makeMove(0,true));
                        //manager.SendMessageToAll(manager.GetGameLogic().makeMove(0,true).c_str());
                        printf("sent hit\n");
                    } else {
                        ClientMessage("Stand");//manager.GetGameLogic().makeMove(0,false));
                        //manager.SendMessageToAll(manager.GetGameLogic().makeMove(0,false).c_str());
                        printf("sent stand\n");
                    }
                    //cout<<"activePlayer: "<<manager.GetGameLogic().getActivePlayer()<<"\nthis player: "<<playerID<<endl;
                    //ClientMessage(manager.GetGameLogic().makeMove(playerID,true));
                    input="";
                    output="";
                }
            //}
        }
    }
}

int main()
{
    ConnectionManager manager;
    //game_logic object;
    //object.updateGame();
    //manager.ServerUpdate();
    return 0;
}
