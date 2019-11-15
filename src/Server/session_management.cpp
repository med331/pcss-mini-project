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
#define BACKLOG 1

#include "game_logic.h"


using namespace std;
//thread object class
//bool doSendMessage = false;
//SOCKET ListenSocket = -1;

class ConnectionThread {
private: //variables

    struct addrinfo *resultCopy;
    int playerID;
    bool playerConnected=false;
    bool isListening=false;
    char* clientMessage;
    SOCKET ListenSocket=-1;
    SOCKET ClientSocket=-1;

public:
    void setResultCopy (struct addrinfo *resultInfo) {resultCopy=resultInfo;}
    void operator() () { //the function used by the threads

        try {ListenSocket = socket(resultCopy->ai_family, resultCopy->ai_socktype, resultCopy->ai_protocol);

        bind( ListenSocket, resultCopy->ai_addr, (int)resultCopy->ai_addrlen);

        freeaddrinfo(const_cast<struct addrinfo*>(resultCopy));

        listen(ListenSocket, BACKLOG);

        ClientSocket = accept(ListenSocket, NULL, NULL);} catch (const std::invalid_argument& e) {printf("problem");}

    }
    void EndThread () {} //Ends the current thread

    void ClientMessage(string input) {
        if(playerConnected) {
            send( ClientSocket, input.c_str(), strlen(input.c_str()), 0 );
        }
    }

    void ListenSocketReset() {
        closesocket(ListenSocket);
    }

    int GetPlayerID() { //getters and setters for playerId
        return playerID;
    }
    void SetPlayerID(int _playerID) {
        playerID=_playerID;
    }

    bool GetPlayerConnected() { //getters and setters for playerId
        return playerConnected;
    }
    void SetPlayerConnected(bool _playerConnected) {
        playerConnected=_playerConnected;
    }

    SOCKET GetClientSocket() { //getters and setters for playerId
        return ClientSocket;
    }
    void SetClientSocket(SOCKET _clientSocket) {
        ClientSocket=_clientSocket;
    }
};

/*class ConnectionAcceptThread {
    void operator() () const { //the function used by the threads
    }
};*/

class ConnectionManager { //Adds and removes players and manages their actions

public:
    static const int maxPlayers=1;

private:

    game_logic game; //State of the game
    ConnectionThread connectionArray[maxPlayers]; //Array of connections
    thread threadArray[maxPlayers]; //Array of threads
    /*ConnectionAcceptThread conAccepter;
    thread conAcceptThread(conAccepter);*/
    WSADATA wsaData;
    int bytesReceived;

    //SOCKET ListenSocket=-1;
    //SOCKET ClientSocket[maxPlayers];

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //int iSendResult;
    char hitOrStandInput[DEFAULT_BUFLEN];
    int inputLength = DEFAULT_BUFLEN;

public:
    ConnectionManager() { //Constructor
        for(int i=0;i<maxPlayers;i++) {
            connectionArray[i].SetPlayerID(i);

        }
        ServerSetup();
    }

    int ServerSetup()
    {
        WSAStartup(MAKEWORD(2,2), &wsaData);

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        connectionArray[0].setResultCopy(result);
        try { threadArray [0].detach();} catch (const std::invalid_argument& e) {printf("problem");}
    }

    void ServerUpdate() {
        //Send current player prompt
        send( connectionArray[game.getActivePlayer()].GetClientSocket(), "It is your turn", 15, 0 );
        //Take input from current player

        bytesReceived = recv(connectionArray[game.getActivePlayer()].GetClientSocket(), hitOrStandInput, inputLength, 0);
        if (bytesReceived > 0) {
            /*printf("Bytes received: %d\n", iResult);
            printf("Message received: %s\n", recvbuf);
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            printf("Bytes sent: %d\n", iSendResult);*/
            //const char* output="deat";
            if(hitOrStandInput=="Hit") {
                SendMessageToAll(game.makeMove(game.getActivePlayer(),true).c_str());
            } else {
                SendMessageToAll(game.makeMove(game.getActivePlayer(),false).c_str());
            }
            /*cout<<bytesReceived<<endl;
            int byteSend=*/
            //send(ClientSocket[activePlayer],output,strlen(output),0);
            //cout<<byteSend<<endl;

            cout<<hitOrStandInput<<endl;
        }
        else if (bytesReceived == 0) {
            //printf("Connection closing...\n");

            Unsubscribe(game.getActivePlayer());
        }
    }

    void Unsubscribe (int playerID) {
        game.removePlayer(playerID);
        connectionArray[playerID].EndThread();
        closesocket(connectionArray[playerID].GetClientSocket());
    } //Removes players

    void Subscribe (int playerID) { //Adds players
        game.addPlayer(playerID);
        threadArray[playerID]=thread(connectionArray[playerID]);
        threadArray[playerID].join();
    }
    //void UpdateConnections () {}

    void ServerClose() {
        WSACleanup();
    }

    void SendMessageToAll(const char* message) {
        for(int i=0;i<maxPlayers;i++) {
            if(connectionArray[i].GetPlayerConnected()) {
                connectionArray[i].ClientMessage(message);
            }
        }
    }
};


/*class ConnectionThread {
private: //variables

    int playerID;
    bool playerConnected=false;
    bool isListening=false;
    string clientMessage;
    static ListenSocket=-1;
    SOCKET ClientSocket=-1;

public:

    void operator() () const { //the function used by the threads
        while(true) {
            if(doSendMessage && playerConnected) {
                send( ClientSocket[playerID], clientMessage, strlen(message), 0 );
                doSendMessage=false;
            }
        }
    }
    void EndThread () {} //Ends the current thread

    void ClientMessage(char* input) {
        doSendMessage=true;
        clientMessage=input;
    }

    void ListenSocketReset() {
    }

    int GetPlayerID() { //getters and setters for playerId
        return playerID;
    }
    void SetPlayerID(int _playerID) {
        playerID=_playerID;
    }

    bool GetPlayerConnected() { //getters and setters for playerId
        return playerConnected;
    }
    void SetPlayerConnected(bool _playerConnected) {
        playerConnected=_playerConnected;
    }
};*/

/*class ConnectionAcceptThread {


    void operator() () const { //the function used by the threads
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        freeaddrinfo(result);

        listen(ListenSocket, BACKLOG);

        ClientSocket = accept(ListenSocket, NULL, NULL);

        for(int i=0;i<ConnectionManager.maxPlayers;i++) {
            ConnectionManager
        }
    }
};*/
