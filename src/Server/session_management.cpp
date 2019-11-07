//class ConnectionManager(){
//private GameState currentGamestate=new GameState()
//
#include <thread>

class ConnectionManager {
    private GameState game;
    private ConnectionThread[] threads

    ConnectionManager() {
        this.game=new GameState();
    }
    public void Unsubscribe (string playerID) {}
    public bool MakeMove (string playerID, string Move) {}
    private void Subscribe (string playerID) {}
    private void UpdateConnections () {}
    public void SetGame (GameState gameInput) {this.game=gameInput;}
    public GameState GetGame () {return this.game;}
}

class ConnectionThread {
    private string playerID;
    private ConnectionManager manager;

    ConnectionThread(string playerID, ConnectionManager manager) {
        this.playerID=playerID;
        this.manager=manager;
    }

    public void operator() () const {

    }
    public void EndThread () {}
}
