#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "./lib/sll.h"
#define MAXLINE 1000
#define BUFF_SIZE 1024
#define SUCCESSFUL 1
#define FALSE 2
#define USER_BLOCKED 3
#define USER_NOT_ACTIVATED 4

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Please input port number\n");
		return 0;
	}
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	char buffer[BUFF_SIZE];
	char *port_number = argv[1];
	int port = atoi(port_number);

	char only_string[BUFF_SIZE];
	char only_number[BUFF_SIZE];

	singlyLinkedList list;
	createSinglyLinkedList(&list);

	int len1, len2;

	readFile(list);

	displaySinglyLinkedList(list);

	// Create a UDP Socket
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == -1)
	{
		perror("Socket creation failed");
		exit(1);
	}
	// Initialize server address structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_INET;

	// Bind the socket to the server address
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("Binding failed");
		close(serverSocket);
		exit(1);
	}

	printf("\nUDP server is running on port %d...\n", port);

	while (1)
	{

		// Receive data from clients
		ssize_t receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

		if (receivedBytes == -1)
		{
			perror("Error receiving data");
			close(serverSocket);
			exit(1);
		}

		// Null-terminate the received data
		buffer[receivedBytes] = '\0';

		printf("Received message from %s:%d: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

		/*
			Check if the account name exists in database
		*/
		if (searchUserByUsername(&list, buffer))
		{
			char *username = buffer;
			
			// If it does exist, send back a message to the client
			strcpy(buffer, "1");
			// Echo the message back to the client
			ssize_t sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);

			// Receive password from client
			receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

			if (searchUser(&list, username, buffer))
			{
				// If password is correct, send back a message to the client
				strcpy(buffer, "1");
				return 0;
			}
			else
			{
				// If password is incorrect, send back a message to the client
				strcpy(buffer, "2");
				return 0;
			}


			if (sentBytes == -1)
			{
				perror("Error sending data");
				close(serverSocket);
				exit(1);
			}
		}
		else
		{
			strcpy(buffer, "2");
		}

		// Echo the message back to the client
		ssize_t sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);

		if (sentBytes == -1)
		{
			perror("Error sending data");
			close(serverSocket);
			exit(1);
		}
	}

	close(serverSocket);
	return 0;
}
