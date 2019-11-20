#include <iostream>
#include "server_interface.cpp"
#include "user_input.cpp"
// convert int to char:  htonl(int);
// convert char to int: ntohl(char)
// https://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
// https://stackoverflow.com/questions/27535543/c-socket-recv-not-receiving-correct-number-of-bytes
// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
int main()
{
    ServerInterface server;
    server.connectToServer();

    bool isPlayerTurn = true;
    while (true) {
        printf("looping");
        if (true) {
            /* Get player input and send it to the server */
            printf("can hitorstand\n");
            isPlayerTurn = false;
            bool hitOrStand = PlayerHit();
            server.sendAction(hitOrStand);
            string response = server.recieveFromServer();
            cout<<response << endl;
        }
        else {
            /* keep receiving messages and update isPlayerTurn if "your turn" message is received" */
            string response = server.recieveFromServer();
            cout<<response << endl;
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
