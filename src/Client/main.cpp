#include "server_interface.cpp"

int main()
{
    ServerInterface server;

    server.connectToServer();
    server.sendToServer();
    server.recieveFromServer();

    return 0;
}
