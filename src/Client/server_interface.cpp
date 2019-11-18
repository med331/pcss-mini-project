#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

#define DEFAULT_ADDRESS "172.20.10.2"

/*using namespace std;*/

class ServerInterface
{
    private:
        WSADATA wsaData;
        SOCKET ConnectSocket = -1;
        struct addrinfo *result = NULL,
                        *ptr = NULL,
                        hints;
        /*const char *sendbuf = "Hello, Network!";*/
        char recvbuf[DEFAULT_BUFLEN];
        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;



    public:
        ServerInterface()
        {
            /* construct me */
            iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

            ZeroMemory( &hints, sizeof(hints) );
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
        };

        void connectToServer()
        {
            printf("This happened");
            for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
                ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                    ptr->ai_protocol);

                iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                if (iResult == -1) {
                    closesocket(ConnectSocket);
                    ConnectSocket = -1;

                }else{
                    break;
                }
            }
            freeaddrinfo(result);
        };

        void sendToServer(const char *message)
        {
            iResult = send( ConnectSocket, message, (int)strlen(message), 0 );
            printf("Bytes Sent: %d\n", iResult);
            /* REMEMBER to add check for message sent */
        };

        /* The message sent to the server when client joins */
        void sendConnectMessage()
        {
            const char *sendbuf = "Client wants to join :)";
            sendToServer(sendbuf);
        };

        /* Actions the client makes, sent to server */
        void sendAction(bool action)
        {
            if (action) {
                const char *sendbuf = "Hit";
                sendToServer(sendbuf);
            }
            else {
                const char *sendbuf = "Stand";
                sendToServer(sendbuf);
            }
        }

        std::string recieveFromServer()
        {
            do {

                iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
                if ( iResult > 0 )
                    /* printf("Bytes received: %d\n", iResult); */
                    return "data received"; /* change this */
                else if ( iResult == 0 )
                    printf("Connection closed\n");
                else
                    printf("recv failed with error: %d\n", WSAGetLastError());

            } while( iResult > 0 );
            return "";
        };

        void closeConnection()
        {
            iResult = shutdown(ConnectSocket, 1);
            closesocket(ConnectSocket);
            WSACleanup();
        }
};
