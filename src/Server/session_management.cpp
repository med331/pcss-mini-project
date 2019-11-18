//class ConnectionManager(){
//private GameState currentGamestate=new GameState()
//
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <thread>
#include <string>
#include <iostream>
#include <sys/types.h>

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
//bool doSendMessage = false;
//SOCKET ListenSocket = -1;


class ConnectionThread {
public: //reset to private

    struct addrinfo *resultCopy;
    int playerID;
    bool playerConnected=true;
    bool isListening=false;
    char* clientMessage;
    SOCKET ListenSocket=-1;
    SOCKET ClientSocket=-1;

public:
    ConnectionThread() {}
    void setResultCopy (struct addrinfo *resultInfo) {resultCopy=resultInfo;}
    void operator() () { //the function used by the threads
            //printf("thread started");
            ListenSocket = socket(resultCopy->ai_family, resultCopy->ai_socktype, resultCopy->ai_protocol);
            //printf("ListenSocket fine");
            bind( ListenSocket, resultCopy->ai_addr, (int)resultCopy->ai_addrlen);
            //printf("Binding fine");
            freeaddrinfo(const_cast<struct addrinfo*>(resultCopy));
            //printf("Freeaddrinfo done & Listening");
            listen(ListenSocket, BACKLOG);
            //printf("Accepting");
            ClientSocket = accept(ListenSocket, NULL, NULL);
            std::cout<<"Ran"<<endl;
            //std::cout<<(int)ClientSocket<<endl;
            playerConnected=true;
            ClientMessage("It is your turn");
            char* input;
            cout<<recv(ClientSocket, input, DEFAULT_BUFLEN, 0)<<"\nMessage: "<<input<<endl;

    }
    void EndThread () {} //Ends the current thread

    void ClientMessage(string input) {
        std::cout<<"Sending"<<endl;
        //if(playerConnected) {
            send( ClientSocket, input.c_str(), DEFAULT_BUFLEN, 0 );
        //}
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

class ConnectionManager { //Adds and removes players and manages their actions

public:
    static const int maxPlayers=1;

private: //reset to private

    game_logic game; //State of the game
    ConnectionThread connectionArray[maxPlayers]; //Array of connections
    std::thread threadArray[maxPlayers];

    WSADATA wsaData;
    int bytesReceived;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //SOCKET ListenSocket=-1;
    //SOCKET ClientSocket=-1;

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
        for(int i=0;i<maxPlayers;i++) {
            threadArray[i]=std::thread(connectionArray[i]);
            threadArray[i].join();
        }
        /*printf("joining");
        //threadArray[0].join();
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        //printf("binding");
        bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        freeaddrinfo(result);
        listen(ListenSocket, BACKLOG);
        printf("listening");
        ClientSocket = accept(ListenSocket, NULL, NULL);
        printf("serverupdate starting");*/
        //cout<<recv(connectionArray[game.getActivePlayer()].GetClientSocket(), hitOrStandInput, inputLength, 0)<<"\nMessage: "<<hitOrStandInput<<endl;
        //while(true) {
            //ServerUpdate();

        //}
    }

    void ServerUpdate() {
        //Send current player prompt
        connectionArray[0].ClientMessage("It is your turn");
        //send( connectionArray[game.getActivePlayer()].GetClientSocket(), "It is your turn", 15, 0 );
        //Take input from current player

        bytesReceived = recv(connectionArray[0].GetClientSocket(), hitOrStandInput, DEFAULT_BUFLEN, 0);
        std::cout<<hitOrStandInput<<endl;
        /*if (bytesReceived > 0) {
            if(hitOrStandInput=="hit") {
                SendMessageToAll(game.makeMove(0,true).c_str());
            } else {
                SendMessageToAll(game.makeMove(0,false).c_str());
            }
            /*cout<<bytesReceived<<endl;
            int byteSend=
            //send(ClientSocket[activePlayer],output,strlen(output),0);
            //cout<<byteSend<<endl;

            cout<<hitOrStandInput<<endl;
        }
        else if (bytesReceived == 0) {
            //printf("Connection closing...\n");

            //Unsubscribe(game.getActivePlayer());
        }*/
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
