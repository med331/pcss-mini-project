#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define DEFAULT_BUFLEN 32
#define DEFAULT_PORT "80"

#define DEFAULT_ADDRESS "127.0.0.1"

using namespace std;

class ServerInterface
{
    private:
        WSADATA wsaData;
        SOCKET ConnectSocket = -1;
        struct addrinfo *result = NULL,
                        *ptr = NULL,
                        hints;
        char* recvbuf;
        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;

    public:
        ServerInterface()
        {
            iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

            ZeroMemory( &hints, sizeof(hints) );
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
        };

        void connectToServer()
        {
            ptr=result;
            //for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
                ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                    ptr->ai_protocol);

                iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
                if (iResult == -1) {
                    closesocket(ConnectSocket);
                    ConnectSocket = -1;

                }else{
                    //break;
                }
            //}
            freeaddrinfo(result);
        };

        void sendToServer(const char *message)
        {
            iResult = send( ConnectSocket, message, (int)strlen(message), 0 );
            printf("Bytes Sent: %d\n", iResult);
            cout << message << endl;
            /* REMEMBER to add check for message sent */
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
        };

        string recieveFromServer()
        {
            do {
                recvbuf = new char[recvbuflen];
                iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
                if ( iResult > 0 ){
                     printf("Bytes received: %d\n", iResult);

                    return recvbuf; /* change this */
                }
                else if ( iResult == 0 )
                    printf("Connection closed\n");
                else
                    printf("recv failed with error: %d\n", WSAGetLastError());

            } while( iResult < recvbuflen );
            return "";
        };

        void closeConnection()
        {
            shutdown(ConnectSocket, 1);
            closesocket(ConnectSocket);
            WSACleanup();
        }
};
