#include "server_interface.cpp"
#include "user_input.cpp"

int main()
{
    ServerInterface server;
    server.connectToServer();

    bool isPlayerTurn = false;
    while (true) {
        if (isPlayerTurn) {
            /* Get player input and send it to the server */
            isPlayerTurn = false;
            bool hitOrStand = PlayerHit();
            server.sendAction(hitOrStand);
        }
        else {
            /* keep receiving messages and update isPlayerTurn if "your turn" message is received" */
            string response = server.recieveFromServer();
            if (response == "your turn")
                isPlayerTurn = true;
            else if (response == "") {
                /* if response is an empty string, the connection has died and we quit */
                printf("Quitting program...");
                break;
            }
            cout << response << endl;
        }
        /* TODO: add a way for the player to exit the application gracefully */
    }

    /* clean up and quit */
    server.closeConnection();
    return 0;
}
