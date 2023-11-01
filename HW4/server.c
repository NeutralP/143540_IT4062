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
#define CORRECT_PASSWORD 5
#define PASSWORD_CHANGED 6

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
	ssize_t sentBytes, receivedBytes;

	char only_string[BUFF_SIZE];
	char only_number[BUFF_SIZE];

	singlyLinkedList list;
	createSinglyLinkedList(&list);
	node *foundUser;

	int loop = 0;

	readFile(&list);

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
		foundUser = NULL;
		loop = 0;
		// Receive data from clients
		receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

		if (receivedBytes == -1)
		{
			perror("Error receiving data");
			close(serverSocket);
			exit(1);
		}

		// Null-terminate the received data
		buffer[receivedBytes] = '\0';

		printf("Received message from %s:%d: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);
		fflush(stdout);

		foundUser = searchUserByUsername(&list, buffer);
		/*
			Check if the account name exists in database
		*/
		if (foundUser)
		{
			char *username = buffer;
			printf("Username: %s\nPassword: %s", foundUser->element.username, foundUser->element.password);
			fflush(stdout);
			// If it does exist, send back a message to the client
			strcpy(buffer, "1");
			// Echo the message back to the client
			sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);

			// Receive password from client
			receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
			// Null-terminate the received data
			buffer[receivedBytes] = '\0';

			char *password = buffer;
			int result = strcmp(password, foundUser->element.password);
			printf("\nPassword: %s %s %d\n", password, foundUser->element.password, result);
			fflush(stdout);
			// Check if password is correct
			if (result == 0)
			{
				printf("Status: %d\n", foundUser->element.status);
				printf("Username: %s\nPassword: %s\n", foundUser->element.username, foundUser->element.password);
				fflush(stdout);
				if (foundUser->element.status == 0)
				{
					printf("\nAccount is blocked.\n");
					// Send the message back to client
					strcpy(buffer, "3");
					// Echo the message back to the client
					sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
				}
				// If account is not activated
				else if (foundUser->element.status == 2)
				{
					printf("\nAccount is not activated.\n");
					// Send the message back to client
					strcpy(buffer, "4");
					// Echo the message back to the client
					sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
				}
				// If account is activated
				else if (foundUser->element.status == 1)
				{
					// If password is correct, send back a message to the client
					strcpy(buffer, "5");
					// Echo the message back to the client
					sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
					// Change password till user input bye
					while (1)
					{
						// Receive password from client
						receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
						// Null-terminate the received data
						buffer[receivedBytes] = '\0';
						if (strcmp(buffer, "bye") == 0)
						{
							break;
						}
						else
						{
							if (foundUser != NULL)
							{
								// Change password
								strcpy(foundUser->element.password, buffer);
								writeFile(&list);
								// If password is correct, send back a message to the client
								strcpy(buffer, "6");
								// Echo the message back to the client
								sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
							}
							else
							{
								printf("Bug.\n");
							}
						}
					}
				}
				loop = 1;
			}
			else if (loop == 0)
			{
				int n = 0;
				while (n < 2)
				{
					// If password is incorrect, send back a message to the client and check
					strcpy(buffer, "2");
					printf("\nNumber of tries: %d\n", n + 1);
					printf("Correct password: %s\n", foundUser->element.password);
					// Echo the message back to the client
					sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
					// Receive password from client
					receivedBytes = recvfrom(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
					// Null-terminate the received data
					buffer[receivedBytes] = '\0';
					if (strcmp(buffer, foundUser->element.password) == 0)
					{
						// If password is correct, send back a message to the client
						strcpy(buffer, "1");
						// Echo the message back to the client
						sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
						break;
					}
					else
					{
						n++;
					}
				}
				if (n == 2)
				{
					// If password is incorrect 3 times, send back a message to the client
					strcpy(buffer, "3");
					// Echo the message back to the client
					sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
					// Change status of account to 0
					if (foundUser != NULL)
					{
						printf("\nBlocking %s", foundUser->element.username);
						foundUser->element.status = 0;
						writeFile(&list);
						printf("\nBlocked %s\n", foundUser->element.username);
						// Send the message back to client
						strcpy(buffer, "3");
						// Echo the message back to the client
						sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
					}
					else
					{
						printf("Account not found.\n");
					}
				}
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
		sentBytes = sendto(serverSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, clientAddrLen);

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
