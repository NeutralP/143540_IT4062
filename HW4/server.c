#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "lib/sll.h"
#include "lib/util.h"
#include "lib/handle_string.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s [PORT_NUMBER]\n", argv[0]);
        return 1;
    }

    int sockfd, rcvBytes, sendBytes;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024] = {0};

    // Read file and create linked list of all users
    singlyLinkedList *list = (singlyLinkedList *)malloc(sizeof(singlyLinkedList));
    createSinglyLinkedList(list);
    readFile(list);
    displaySinglyLinkedList(list);

    node *e = (node *)malloc(sizeof(node));
    if ((e = searchUserByUsername(list, "hedspi23")) == NULL)
    {
        printf("\nsearchUserByUsername is behaving correctly");
    }

    if ((e = searchUser(list, "hedspi2", "WRONG")) == NULL)
    {
        printf("\nsearchUser is behaving correctly");
    }
    if ((e = searchUser(list, "sdfasdf", "hust1")) != NULL)
    {
        printUser(e);
    }
    else
    {
        printf("\nsearchUser is behaving correctly when input wrong password");
    }

    printf("\nServer is ready. Waiting for a client...\n");
    fflush(stdout);

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));

    // Forcefully attaching socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        len = sizeof(cliaddr);
        memset(buffer, 0, sizeof(buffer)); // clear buffer

        // Receive username from client
        rcvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
        buffer[rcvBytes] = '\0';
        char tmp_username[1024];
        strcpy(tmp_username, buffer);
        printf("SERVER: Received username.\n");

        printf("TO_CLIENT: Requesting password..\n");
        sendto(sockfd, REQUEST_PASSWORD, strlen(REQUEST_PASSWORD), 0, (struct sockaddr *)&cliaddr, len);

        // Receive password from client
        memset(buffer, 0, sizeof(buffer)); // clear buffer
        rcvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
        buffer[rcvBytes] = '\0';

        printf("SERVER: Received password.\n");
        char tmp_password[1024];
        strcpy(tmp_password, buffer);
        printf("SERVER: Received username / password: %s %s\n", tmp_username, tmp_password);

        node *p = (node *)malloc(sizeof(node));

        p = searchUserByUsername(list, tmp_username);
        if (p == NULL)
        {
            printf("\nWrong password\n");
            printf("Sending message to Client\n");
            sendto(sockfd, USER_NOT_FOUND, strlen(USER_NOT_FOUND), 0, (struct sockaddr *)&cliaddr, len);
            break;
        }

        p = searchUser(list, tmp_username, tmp_password);
        if (p == NULL)
        {
            int n = 0;
            while ((p = searchUser(list, tmp_username, tmp_password)) == NULL && n++ < 2)
            {
                // Send message to client
                printf("\nUser not found\n");
                printf("Sending message to Client\n");
                sendto(sockfd, INVALID_PASSWORD, strlen(INVALID_PASSWORD), 0, (struct sockaddr *)&cliaddr, len);

                // Receive password from client
                memset(buffer, 0, sizeof(buffer)); // clear buffer
                rcvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
                buffer[rcvBytes] = '\0'; // add '\0' after received
                printf("SERVER: Received password: %s\n", buffer);
                strcpy(tmp_password, buffer);

                printf("\n%d", n);
            }

            printf("\n%d", n);

            if (n > 2)
            {
                printf("\nTO_CLIENT: Sending message to Client\n");
                sendto(sockfd, USER_BLOCKED, strlen(USER_BLOCKED), 0, (struct sockaddr *)&cliaddr, len);
                e = searchUserByUsername(list, tmp_username);
                e->element.status = 0;
                writeFile(list);
                break;
            }
        }
        if (p != NULL)
        {
            switch (p->element.status)
            {
            case 0:
                printf("\nCorrect credentials but user status is blocked.\n");
                sendto(sockfd, USER_BLOCKED, strlen(USER_BLOCKED), 0, (struct sockaddr *)&cliaddr, len);
                break;
            case 1:
                printf("\nUser status is active.\n");
                sendto(sockfd, USER_FOUND, strlen(USER_FOUND), 0, (struct sockaddr *)&cliaddr, len);

                while (1)
                {
                    // Receive new password from client
                    memset(buffer, 0, sizeof(buffer));
                    rcvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
                    buffer[rcvBytes] = '\0';
                    printf("\nCLIENT: Received message: %s\n", buffer);
                    if (strcmp(buffer, "bye") == 0)
                        break;
                    char received_alpha[1024];
                    char received_num[1024];
                    int a = breakPassword(buffer, received_alpha, received_num);
                    if (a == 1)
                    {
                        printf("\nPassword is valid\n");
                        // Send response to client
                        printf("TO_CLIENT: Sending message to Client\n");
                        printf("TO_CLIENT: %s %s\n", received_alpha, received_num);

                        sendBytes = sendto(sockfd, VALID_PASSWORD, strlen(VALID_PASSWORD), 0, (struct sockaddr *)&cliaddr, len);
                        printf("New credentials: %s %s\n", p->element.username, buffer);
                        strcpy(p->element.password, buffer);
                        writeFile(list);

                        memset(buffer, 0, sizeof(buffer));
                        rcvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
                        buffer[rcvBytes] = '\0';
                        if (strlen(received_alpha) == 0)
                            sendto(sockfd, EMPTY_STRING, strlen(EMPTY_STRING), 0, (struct sockaddr *)&cliaddr, len);
                        else
                            sendto(sockfd, received_alpha, strlen(received_alpha), 0, (struct sockaddr *)&cliaddr, len);

                        memset(buffer, 0, sizeof(buffer));
                        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
                        if (strlen(received_num) == 0)
                            sendto(sockfd, EMPTY_STRING, strlen(EMPTY_STRING), 0, (struct sockaddr *)&cliaddr, len);
                        else
                            sendto(sockfd, received_num, strlen(received_num), 0, (struct sockaddr *)&cliaddr, len);
                    }
                    else if (a == 0)
                    {
                        printf("\nPassword is invalid\n");
                        sendto(sockfd, INVALID_PASSWORD, strlen(INVALID_PASSWORD), 0, (struct sockaddr *)&cliaddr, len);
                    }
                }
                break;
            case 2:
                printf("\nUser status is new.\n");
                sendto(sockfd, USER_NOT_ACTIVATED, strlen(USER_NOT_ACTIVATED), 0, (struct sockaddr *)&cliaddr, len);
                break;
            default:
                break;
            }
            break;
        }
    }
    return 0;
}
