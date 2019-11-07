//class ConnectionManager(){
//private GameState currentGamestate=new GameState()
//
#include <thread>
#include <string>
#include <iostream>

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

class GameState {}; //temporary while it is being created in game logic

class ConnectionManager { //Adds and removes players and manages their actions
private:
    GameState game; //State of the game
    ConnectionThread connectionArray[4]; //Array of connections
    thread threadArray[4]; //Array of threads


public:
    ConnectionManager() { //Constructor
        for(int i=0;i<4;i++) {
            connectionArray[i].SetPlayerID(i);
        }
    }

    void Unsubscribe (int playerID) {
        game.removePlayer(playerID);
        connectionArray[playerID].EndThread();
    } //Removes players

    bool MakeMove (int playerID, bool Move) { //Handles player actions
        return game.makeMove(playerID, Move);
    }

    //Setters and getters for gameState
    void SetGame (GameState gameInput) {game=gameInput;}
    GameState GetGame () {return game;}

    void Subscribe (int playerID) { //Adds players
        game.addPlayer(playerID);
        threadArray[playerID]=thread(connectionArray[playerID]);
        threadArray[playerID].join();
    }
    //void UpdateConnections () {}
};
