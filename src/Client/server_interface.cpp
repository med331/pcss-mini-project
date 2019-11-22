#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define DEFAULT_BUFLEN 512 /* set the maximum packet size */
#define DEFAULT_PORT "80" /* use port 80, the standard port for HTTP traffic, thereby not requiring any port-forwarding */

#define DEFAULT_ADDRESS "127.0.0.1" /* the hard-coded IP address of the test server */

using namespace std;

class ServerInterface
{
    private:
        /* variables described during initialization in class constructor */
        WSADATA wsaData;
        SOCKET ConnectSocket = -1;
        struct addrinfo *result = NULL,
                        *ptr = NULL,
                        hints;
        char* recvbuf;
        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;

    public:
        /**
         *  Initialize object of type ServerInterface
         */
        ServerInterface()
        {
            iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

            ZeroMemory( &hints, sizeof(hints) );        /* clean out struct */
            hints.ai_family = AF_UNSPEC;                /* IPv4 or IPv6 does not matter */
            hints.ai_socktype = SOCK_STREAM;            /* TCP streaming socket */

            iResult = getaddrinfo(DEFAULT_ADDRESS,      /* hostname, in this case the remote server IP address */
                                  DEFAULT_PORT,         /* port 80 */
                                  &hints, &result);     /* pointer to addrinfo and other relevant information */
        };

        /**
         *  Attempt to connect to remote server
         */
        void connectToServer()
        {
            // establish socket
            ptr=result;
            ConnectSocket = socket(ptr->ai_family, 
                                   ptr->ai_socktype, 
                                   ptr->ai_protocol);
            // attempt connection
            iResult = connect(ConnectSocket, 
                              ptr->ai_addr, 
                              (int)ptr->ai_addrlen);
            // give up if connection attempt fails
            if (iResult == -1) {
                closesocket(ConnectSocket);
                ConnectSocket = -1;
            }
            // cleanup
            freeaddrinfo(result);
        };

        /**
         *  Send a list of chars to the server
         */
        void sendToServer(const char *message)
        {
            iResult = send( ConnectSocket, message, (int)strlen(message), 0 );
            cout << message << endl;
        };

        /**
         *  Wrapper function for sendToServer, specifically handling hit or stand
         */
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

        /**
         *  Receive server event or empty string if no longer connected
         */
        string recieveFromServer()
        {
            do {
                recvbuf = new char[recvbuflen];
                iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
                // return data if there is any
                if ( iResult > 0 ) {
                    return recvbuf;
                }
                // error handling
                else if ( iResult == 0 )
                    printf("Connection closed\n");
                else
                    printf("recv failed with error: %d\n", WSAGetLastError());

            } while( iResult > 0 );
            return "";
        };

        /**
         *  Close server connection and clean up
         */
        void closeConnection()
        {
            shutdown(ConnectSocket, 1);
            closesocket(ConnectSocket);
            WSACleanup();
        }
};
