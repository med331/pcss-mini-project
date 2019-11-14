//class ConnectionManager(){
//private GameState currentGamestate=new GameState()
//
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <thread>
#include <string>
#include <iostream>
//#include <sys/types.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"
#define BACKLOG 10

#include "game_logic.h"


using namespace std;
//thread object class
class ConnectionThread {
private: //variables

    int playerID;

public:

    void operator() () const { //the function used by the threads
        cout<<"hi"<<endl;
    }
    void EndThread () {} //Ends the current thread
    int GetPlayerID() { //getters and setters for playerId
        return playerID;
    }
    void SetPlayerID(int _playerID) {
        playerID=_playerID;
    }
};

class ConnectionManager { //Adds and removes players and manages their actions

public:
    static const int maxPlayers=1;

private:

    static const game_logic game; //State of the game
    ConnectionThread connectionArray[maxPlayers]; //Array of connections
    thread threadArray[maxPlayers]; //Array of threads
    /*int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";*/
    WSADATA wsaData;
    int iResult;
    int bytesReceived;

    SOCKET ListenSocket=-1 ;
    SOCKET ClientSocket=-1 ;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //int iSendResult;
    char hitOrStandInput[DEFAULT_BUFLEN];
    int inputLength = DEFAULT_BUFLEN;

public:
    ConnectionManager() { //Constructor
        ServerSetup();
        for(int i=0;i<maxPlayers;i++) {
            connectionArray[i].SetPlayerID(i);

        }
    }

    int ServerSetup()
    {
        //iResult =
        WSAStartup(MAKEWORD(2,2), &wsaData);

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        //iResult =
        getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        //iResult =
        bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        freeaddrinfo(result);

        //iResult =
        listen(ListenSocket, BACKLOG);

        ClientSocket = accept(ListenSocket, NULL, NULL);
        //closesocket(ListenSocket);

        //iResult =
        //shutdown(ClientSocket, 1);

    }

    void ServerUpdate() {
        bytesReceived = recv(ClientSocket, hitOrStandInput, inputLength, 0);
        //if (bytesReceived > 0) {
            /*printf("Bytes received: %d\n", iResult);
            printf("Message received: %s\n", recvbuf);
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            printf("Bytes sent: %d\n", iSendResult);*/
            const char* output="death";
            if(hitOrStandInput=="Hit") {
                //output=game.makeMove(0,true);
            } else {
                //output=game.makeMove(0,false);
            }
            cout<<bytesReceived<<endl;
            int byteSend=send(ClientSocket,output,strlen(output),0);
            cout<<byteSend<<endl;

            cout<<hitOrStandInput<<endl;
        /*}
        else if (bytesReceived == 0)
            //printf("Connection closing...\n");
            */closesocket(ClientSocket);

    }

    void Unsubscribe (int playerID) {
        //game.removePlayer(playerID);
        connectionArray[playerID].EndThread();
    } //Removes players

    /*bool MakeMove (int playerID, bool Move) { //Handles player actions
        return game.makeMove(playerID, Move);
    }*/

    //Setters and getters for gameState
    /*void SetGame (GameState gameInput) {game=gameInput;}
    GameState GetGame () {return game;}*/

    void Subscribe (int playerID) { //Adds players
        //game.addPlayer(playerID);
        threadArray[playerID]=thread(connectionArray[playerID]);
        threadArray[playerID].join();
    }
    //void UpdateConnections () {}

    void ServerClose() {
        WSACleanup();
    }
};
