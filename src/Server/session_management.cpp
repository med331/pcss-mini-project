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

	int playerID;
	bool playerConnected = false;
	char clientMessage[DEFAULT_BUFLEN];
	SOCKET ClientSocket = -1;
	int bytesReceived = 0;

	int ClientMessage(string _sendInput) {
		const char* _input=_sendInput.c_str();
		bytesReceived = send(ClientSocket, _input, DEFAULT_BUFLEN, 0);
		return bytesReceived;
	}

	string ClientReceive() {
		char* _input = new char[DEFAULT_BUFLEN];
		bytesReceived = recv(ClientSocket, _input, DEFAULT_BUFLEN, 0);
		return _input;
	}


public:
	void operator() (SOCKET _ClientSocket);//the function used by the threads

	int GetPlayerID() { //getters and setters for playerId
		return playerID;
	}

	void SetPlayerID(int _playerID) {
		playerID = _playerID;
	}

	void SetClientSocket(SOCKET _clientSocket) {
		ClientSocket = _clientSocket;
	}

	void SendMessageToAll(const char* _message);
};


class ConnectionManager { //Adds and removes players and manages their actions

private:

	ConnectionThread connection; //Array of connections
	std::thread thread;

	WSADATA wsaData;
	int bytesReceived;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	SOCKET ListenSocket = -1;
	SOCKET ClientTempSocket = -1;

	int inputLength = DEFAULT_BUFLEN;

public:

	ConnectionManager() {} //Constructor

	int ServerSetup()
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		freeaddrinfo(result);
		listen(ListenSocket, BACKLOG);
		while (true) {
			ClientTempSocket = accept(ListenSocket, NULL, NULL);
			Subscribe(ClientTempSocket);
			printf("Player succesfully subscribed\n");
		}
	}

	void Unsubscribe(SOCKET clientSocket) { //Removes players and exits the code
		closesocket(clientSocket);
		thread=std::thread();
		WSACleanup();
		exit(0);
	}

	void Subscribe(SOCKET clientSocket) { //Adds players
		thread = std::thread(connection, clientSocket);
		thread.detach();
	}
};
