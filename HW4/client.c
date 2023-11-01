// udp client driver program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "./lib/sll.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
	// catch wrong input
	if (argc != 3)
	{
		printf("Please input IP address and port number\n");
		return 0;
	}
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int clientSocket;
	char buffer[BUFF_SIZE];
	char message[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	char INSERT_PASSWORD[] = "\nInsert password: ";
	char OK[] = "\nOK\n";
	char NOT_OK[] = "\nNot OK\n";
	char ACCOUNT_IS_BLOCKED[] = "\nAccount is blocked\n";
	char s5[] = "\nAccount not activated\n";
	char ERROR_PROMPT[] = "\nError: Password includes special characters\n";
	char END_KEY[] = "\nbye";
	char END_MSG[] = "\nGoodbye hust\n";
	char only_string[BUFF_SIZE];
	char only_number[BUFF_SIZE];
	ssize_t sentBytes, receivedBytes;
	size_t len;
	socklen_t serverAddrLen;

	// Create UDP socket
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == -1)
	{
		perror("Socket creation failed");
		exit(1);
	}

	// Initialize server address structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_addr.s_addr = inet_addr(ip_address);
	serverAddr.sin_port = htons(port);
	serverAddr.sin_family = AF_INET;

	// connect to server
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		exit(0);
	}

	while (1)
	{
		memset(buffer, 0, BUFF_SIZE);
		printf("Enter username: ");
		fgets(buffer, BUFF_SIZE, stdin);

		/*
			Exit code example
		*/
		// if (strcmp(buffer, "exit\n") == 0)
		// {
		// 	break; // Exit the loop
		// }

		// Remove the newline character
		len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
		{
			buffer[len - 1] = '\0';
		}

		// Send the message to the server
		sentBytes = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

		if (sentBytes == -1)
		{
			perror("Error while sending data");
			break;
		}

		// printf("Sent message to the server: %s\n", buffer);

		// Receive response from server
		serverAddrLen = sizeof(serverAddr);
		receivedBytes = recvfrom(clientSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLen);

		if (receivedBytes == -1)
		{
			perror("Error receiving data");
			close(clientSocket);
			exit(1);
		}

		// Null-terminate the received data
		buffer[receivedBytes] = '\0';
		// Print received message
		printf("1.Received message from the server: %s\n", buffer);

		// If buffer returned 1 then ask for password else, keep looping
		if (strcmp(buffer, "1") == 0)
		{
			while (1)
			{
				// Ask for password
				printf("%s", INSERT_PASSWORD);
				fgets(buffer, BUFF_SIZE, stdin);

				// Remove the newline character
				len = strlen(buffer);
				if (len > 0 && buffer[len - 1] == '\n')
				{
					buffer[len - 1] = '\0';
				}

				// Send the message to the server
				sentBytes = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

				if (sentBytes == -1)
				{
					perror("Error while sending data");
					break;
				}

				// Receive response from server
				serverAddrLen = sizeof(serverAddr);
				receivedBytes = recvfrom(clientSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLen);

				if (receivedBytes == -1)
				{
					perror("Error receiving data");
					close(clientSocket);
					exit(1);
				}

				// Null-terminate the received data
				buffer[receivedBytes] = '\0';
				// Print received message
				printf("2.Received message from the server: %s\n", buffer);

				

				// If received message is 5 then ask for new password
				if (strcmp(buffer, "5") == 0)
				{
					while (1)
					{
						printf("\nHello user");
						printf("\nInsert new password: (say bye to exit): ");
						// Check if password inputted is correct format (no space, only numbers and alphabet characters), if correct then proceed if not then loop for new password
						while (1)
						{
							fgets(buffer, BUFF_SIZE, stdin);
							// Remove the newline character
							len = strlen(buffer);
							if (len > 0 && buffer[len - 1] == '\n')
							{
								buffer[len - 1] = '\0';
							}
							// Split string to number and string
							int result = split(buffer, only_number, only_string);
							// If result is 1 then password is correct format
							if (result == 1)
							{
								break;
							}
							else
							{
								printf("%s", ERROR_PROMPT);
							}
						}




						// Send the message to the server
						sentBytes = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
						if (sentBytes == -1)
						{
							perror("Error while sending data");
							break;
						}
						if (strcmp(buffer, "bye") == 0)
						{
							printf("%s", END_MSG);
							return 0;
						}
						// Receive response from server
						serverAddrLen = sizeof(serverAddr);
						receivedBytes = recvfrom(clientSocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&serverAddr, &serverAddrLen);
						// Null-terminate the received data
						buffer[receivedBytes] = '\0';
						// Print received message
						printf("3.Received message from the server: %s\n", buffer);
					}
					close(clientSocket);
					return 0;
				}
				if (strcmp(buffer, "3") == 0) {
					printf("%s", ACCOUNT_IS_BLOCKED);
					close(clientSocket);
					return 0;
				}
			}
		}
		else if (strcmp(buffer, "2") == 0)
		{
			printf("%s", NOT_OK);
		}
		else if (strcmp(buffer, "3") == 0)
		{
			printf("%s", ACCOUNT_IS_BLOCKED);
		}
	}
	// Close the socket
	close(clientSocket);

	return 0;
}
