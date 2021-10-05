/*CODE FOR SERVER v1
Rkannan_octaOmega*/


#include<stdio.h>
#include<io.h>
#include<winsock2.h>
#include<Ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <processthreadsapi.h>
#include <process.h>
#include <tchar.h>
#include <stdbool.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


int main(int argc, char* argv[])

{

    WSADATA wsa;
    struct sockaddr_in serv; //This is our main socket variable.

    int fd; //This is the socket file descriptor that will be used to identify the socket
    int conn; //This is the connection file descriptor that will be used to distinguish client connections.
    char message[100] = ""; //This array will store the messages that are sent by the server

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL bProcess;

    ZeroMemory(&si, sizeof(si)); //Fills si & pi with zeros.
    ZeroMemory(&pi, sizeof(pi));

    char ipadd[33];
    int _portNo;

    printf("\nInitialising Winsock...\n");

    printf("Enter the IP address to configure SERVER: [127.0.0.1 if LocalHost]:\n");
    fgets(ipadd, sizeof(ipadd), stdin);

    printf("Enter PortNo to listern for connections: ");
    scanf_s("%d", &_portNo);
    printf("\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    serv.sin_family = AF_INET;
    serv.sin_port = htons(_portNo); //Define the port at which the server will listen for connections.
    inet_pton(AF_INET, ipadd, &(serv.sin_addr.s_addr));
    fd = socket(AF_INET, SOCK_STREAM, 0); //This will create a new socket and also return the identifier of the socket into fd.

    //To handle errors, if condition that checks whether fd is greater than 0. If it isn't, prompt an error
    //Assigns the address specified by serv to the socket

    if (bind(fd, (struct sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    //Listen for client connections
    // Maximum 5 connections will be permitted.

    listen(fd, 5); 

    //Handling the connections.

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    while (conn = accept(fd, (struct sockaddr*)NULL, NULL)) {
        int pid;
        while (recv(conn, message, 100, 0) > 0) {
            if ((pid = (bProcess = (CreateProcess(NULL,   // No module name (use command line)
                NULL,        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi)           // Pointer to PROCESS_INFORMATION structure
                )) == 0)) {
                printf("Message Received From CLIENT: %s\n", message);

                //Breaking condition to terminate the child process

                if (message[0] == 'e' || message[0] == 'E') {
                 printf("::::Client Terminated : Exit....:::\n");
                 _close(fd);
                 return 0;
                }
            }
            memset(message, 0, sizeof(message));
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    return 0;
}
