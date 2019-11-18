#include "server_interface.cpp"
#include "user_input.cpp"

int main()
{
    ServerInterface server;

    server.connectToServer();
    server.sendConnectMessage();
    server.sendAction(true);
    /* after an arbitrary number of message, close the connection */

    bool isPlayerTurn = false;
    while (true) {
        if (isPlayerTurn) {
            isPlayerTurn = false;
            /* Get player input and send to server */
            bool hitOrStand = PlayerHit();
            server.sendAction(hitOrStand);
        }
        else {
            /* keep receiving messages and update isPlayerTurn if "your turn" message is received" */
            string response = server.recieveFromServer();
            if (response == "your turn") /* this will probably change */
                isPlayerTurn = true;
            cout << response << endl;
        }
    }

    server.closeConnection();

    return 0;
}
