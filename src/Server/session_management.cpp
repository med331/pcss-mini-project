//class ConnectionManager(){
//private GameState currentGamestate=new GameState()
//
#include <thread>
#include <string>
#include <iostream>

using namespace std;

class ConnectionThread {
private:
    string playerID;
public:

    void operator() () const {
        cout<<"hi"<<endl;
    }
    void EndThread () {}
    string GetPlayerID() {
        return playerID;
    }
    void SetPlayerID(string _playerID) {
        playerID=_playerID;
    }
};

class GameState {};

class ConnectionManager {
private:
    GameState game;
    ConnectionThread connectionArray[4];
    thread threadArray[4];


public:
    ConnectionManager() {
        for(int i=0;i<4;i++) {
            connectionArray[i].SetPlayerID(to_string(i));
        }
    }

    void Unsubscribe (string playerID) {}
    bool MakeMove (string playerID, string Move) {return true;}
    void SetGame (GameState gameInput) {game=gameInput;}
    GameState GetGame () {return game;}
    void Subscribe (string playerID) {
        threadArray[stoi(playerID)]=thread(connectionArray[stoi(playerID)]);
        threadArray[stoi(playerID)].join();
    }
    //void UpdateConnections () {}
};
