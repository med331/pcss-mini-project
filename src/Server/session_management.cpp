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


class ConnectionThread {
private:

//  Declaring all class variables
	int playerID;
	char clientMessage[DEFAULT_BUFLEN];
	SOCKET ClientSocket = -1;
	int bytesReceived = 0;
	bool playerConnected;
// Sends input to client
	int ClientMessage(string _sendInput) {
		const char* _input=_sendInput.c_str();
		bytesReceived = send(ClientSocket, _input, DEFAULT_BUFLEN, 0);
		return bytesReceived;
	}
// Receives message from client
	string ClientReceive() {
		char* _input = new char[DEFAULT_BUFLEN];
		bytesReceived = recv(ClientSocket, _input, DEFAULT_BUFLEN, 0);
		return _input;
	}


public:
	void operator() (SOCKET _ClientSocket);//the function used by the threads

//Getters and setters for playerId
	int GetPlayerID() {
		return playerID;
	}

	void SetPlayerID(int _playerID) {
		playerID = _playerID;
	}
// Setter for ClientSocket
	void SetClientSocket(SOCKET _clientSocket) {
		ClientSocket = _clientSocket;
	}

};


class ConnectionManager { //Adds and removes player and manages their actions

private:

	ConnectionThread connection; //Connection
	std::thread thread;
// Winsock data
	WSADATA wsaData;
	int bytesReceived;

// Address info
	struct addrinfo* result = NULL;
	struct addrinfo hints;

// Sockets
	SOCKET ListenSocket = -1;
	SOCKET ClientTempSocket = -1;

public:

	ConnectionManager() {} //Constructor for ConnectionManager

	int ServerSetup()
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData); // Starts Winsock

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;             // Sets sockettype
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // Setup of listening socket
		bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen); // Binds socket to address
		freeaddrinfo(result);
		listen(ListenSocket, BACKLOG); // Starts listening
		while (true) {
			ClientTempSocket = accept(ListenSocket, NULL, NULL); // Waits for connection to be accepted
			Subscribe(ClientTempSocket); // Adds player to game, using Subscribe function
			printf("Player succesfully subscribed\n");
		}
	}
    //Removes player and exits the program
	void Unsubscribe(SOCKET clientSocket) {
		closesocket(clientSocket);
		thread=std::thread();
		WSACleanup();
		exit(0);
	}
    //Adds player
	void Subscribe(SOCKET clientSocket) {
		thread = std::thread(connection, clientSocket);
		thread.detach();
	}
};
