/*Rajesh Kannan
client v1
March 28 2021*/


#include<stdio.h>
#include<io.h>
#include<winsock2.h>
#include<Ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <processthreadsapi.h>
#include <process.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET fd; //This is the socket file descriptor that will be used to identify the socket

	struct sockaddr_in serv; //This is our main socket variable.

	// int fd; //This is the socket file descriptor that will be used to identify the socket
	
	int conn; //This is the connection file descriptor that will be used to distinguish client connections.
	char message[100] = ""; //This array will store the messages that are sent by the server

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket 

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	char ipadd[33];
	int _portNo;

	//This binds the client to server IP

	printf("Enter Server IP to connect: ");
	fgets(ipadd, sizeof(ipadd), stdin);
	printf("\n");
	
	printf("Enter Server listening port number: ");
	scanf_s("%d", &_portNo);

	inet_pton(AF_INET, ipadd, &(serv.sin_addr)); 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(_portNo);
	
	//This connects the client to the server.

	if (connect(fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
		printf("**Connection ERROR : SERVER is not Listening**\n"); 
		return 1;
	}
	printf(":::::SERVER Connected: ENTER ""E"" to EXIT::::::\n");

	while (1) {
		printf("Enter a message to SERVER: ");
		fgets(message, 100, stdin);
		send(fd, message, strlen(message), 0);
		 
		 //Breaking condition to terminate the client process
		 
		if (message[0] == 'e' || message[0] == 'E') {
			printf("Client Exit....\n");
			_close(fd);
			return 0;
		}
	}
	return 0;
}