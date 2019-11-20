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

#define DEFAULT_BUFLEN 32
#define DEFAULT_PORT "80"
#define BACKLOG 10

#include "game_logic.h"

using namespace std;


class ConnectionThread {
public: //reset to private

	int playerID;
	bool playerConnected = false;
	char clientMessage[DEFAULT_BUFLEN];
	SOCKET ClientSocket = -1;
	int bytesReceived = 0;

	int ClientMessage(string _input) {
		//if(playerConnected) {
		cout << "Sent " << bytesReceived << " bytes" << endl;
		cout << "output: " << _input << endl;
		bytesReceived = send(ClientSocket, _input.c_str(), (int)strlen(_input.c_str()), 0);

		return bytesReceived;
		//}
	}

	string ClientReceive() {
		char* _input = new char[DEFAULT_BUFLEN];
		//if(playerConnected) {
		bytesReceived = recv(ClientSocket, _input, (int)strlen(_input), 0);
		return _input;
		//}
	}


public:
	void operator() (SOCKET _ClientSocket);//the function used by the threads

	void EndThread() {} //Ends the current thread




	int GetPlayerID() { //getters and setters for playerId
		return playerID;
	}

	void SetPlayerID(int _playerID) {
		playerID = _playerID;
	}

	/*void SetGameReference(game_logic _gameRef) {
		gameReference=_gameRef;
	}*/

	bool GetPlayerConnected() { //getters and setters for playerId
		return playerConnected;
	}

	void SetPlayerConnected(bool _playerConnected) {
		playerConnected = _playerConnected;
	}

	SOCKET GetClientSocket() { //getters and setters for playerId
		return ClientSocket;
	}

	void SetClientSocket(SOCKET _clientSocket) {
		ClientSocket = _clientSocket;
	}

	void NextTurn() {

	}

	void SendMessageToAll(const char* _message);
};


class ConnectionManager { //Adds and removes players and manages their actions

private: //reset to private

	int playerAmount = 0;

	game_logic game; //State of the game
	ConnectionThread connectionArray[4]; //Array of connections
	std::thread threadArray[4];

	WSADATA wsaData;
	int bytesReceived;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	SOCKET ListenSocket = -1;
	SOCKET ClientTempSocket = -1;

	int inputLength = DEFAULT_BUFLEN;

public:
	ConnectionManager() { //Constructor
		for (int i = 0; i < 4; i++) {
			connectionArray[i].SetPlayerID(i);
			//connectionArray[i].SetGameReference(game);
		}
		ServerSetup();
	}

	int ServerSetup()
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		//printf("joining");
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		//printf("binding");
		bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		freeaddrinfo(result);
		listen(ListenSocket, BACKLOG);
		//printf("listening");
		//printf("serverupdate starting\n");
		GameSetup();
		while (true) {
			ClientTempSocket = accept(ListenSocket, NULL, NULL);
			//printf("accepted\n");
			Subscribe(playerAmount, ClientTempSocket);
			//printf("succesfully subscribed\n");
			//SendMessageToAll((const char*)"fun");
		}
	}

	void GameSetup() {
		game.updateGame();
	}

	/*void GameUpdate(char hitOrStandInput[DEFAULT_BUFLEN]) {
		//Send current player prompt
		connectionArray[0].ClientMessage("It is your turn");
		//send( connectionArray[game.getActivePlayer()].GetClientSocket(), "It is your turn", 15, 0 );
		//Take input from current player
		//bytesReceived = recv(connectionArray[0].GetClientSocket(), hitOrStandInput, DEFAULT_BUFLEN, 0);
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
		}
	}*/

	void Unsubscribe(int playerID) {
		connectionArray[playerID].EndThread();
		closesocket(connectionArray[playerID].GetClientSocket());
		game.removePlayer(playerID);
		playerAmount--;
	} //Removes players

	void Subscribe(int playerID, SOCKET clientSocket) { //Adds players
		threadArray[playerID] = std::thread(connectionArray[playerID], clientSocket);
		threadArray[playerID].detach();
		game.addPlayer(playerID);
		playerAmount++;
	}

	void ServerClose() {
		WSACleanup();
	}

	void SendMessageToAll(const char* message) {
		for (int i = 0; i < 4; i++) {
			if (connectionArray[i].GetPlayerConnected()) {
				connectionArray[i].ClientMessage(message);
			}
		}
	}

	game_logic GetGameLogic() {
		return game;
	}
};



/*void ConnectionThread::SendMessageToAll(const char* _message) {
		for(int i=0;i<4;i++) {
			if(connectionArray[i].GetPlayerConnected()) {
				connectionArray[i].ClientMessage(_message);
			}
		}
	}*/


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