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

    int server_fd, new_socket, valread;
    struct sockaddr_in server;
    int opt = 1;
    int addrlen = sizeof(server);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

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
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the specified port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    // Forcefully attaching socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("\n\nSERVER: Client connected: %s\n", inet_ntoa(server.sin_addr));
        while (1)
        {
            memset(buffer, 0, sizeof(buffer)); // clear buffer

            // Receive username from client
            valread = recv(new_socket, buffer, sizeof(buffer), 0);
            buffer[valread] = '\0';
            char tmp_username[1024];
            strcpy(tmp_username, buffer);
            printf("SERVER: Received username.\n");

            printf("TO_CLIENT: Requesting password..\n");
            send(new_socket, REQUEST_PASSWORD, strlen(REQUEST_PASSWORD), 0);

            // Receive password from client
            memset(buffer, 0, sizeof(buffer)); // clear buffer
            valread = recv(new_socket, buffer, sizeof(buffer), 0);
            buffer[valread] = '\0';

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
                send(new_socket, USER_NOT_FOUND, strlen(USER_NOT_FOUND), 0);
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
                    send(new_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD), 0);

                    // Receive password from client
                    memset(buffer, 0, sizeof(buffer)); // clear buffer
                    valread = recv(new_socket, buffer, sizeof(buffer), 0);
                    buffer[valread] = '\0'; // add '\0' after received
                    printf("SERVER: Received password: %s\n", buffer);
                    strcpy(tmp_password, buffer);

                    printf("\n%d", n);
                }

                printf("\n%d", n);

                if (n > 2)
                {
                    printf("\nTO_CLIENT: Sending message to Client\n");
                    send(new_socket, USER_BLOCKED, strlen(USER_BLOCKED), 0);
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
                    send(new_socket, USER_BLOCKED, strlen(USER_BLOCKED), 0);
                    break;
                case 1:
                    printf("\nUser status is active.\n");
                    send(new_socket, USER_FOUND, strlen(USER_FOUND), 0);

                    while (1)
                    {
                        // Receive new password from client
                        memset(buffer, 0, sizeof(buffer));
                        valread = recv(new_socket, buffer, sizeof(buffer), 0);
                        buffer[valread] = '\0';
                        printf("\nCLIENT: Received message: %s\n", buffer);
                        if (strcmp(buffer, "bye") == 0)
                            break;
                        char received_alpha[1024];
                        char received_num[1024];
                        int a = breakPassword(buffer, received_alpha, received_num);
                        if (a)
                        {
                            printf("\nPassword is valid\n");
                            // Send response to client
                            printf("TO_CLIENT: Sending message to Client\n");
                            printf("TO_CLIENT: %s %s\n", received_alpha, received_num);
                            send(new_socket, VALID_PASSWORD, strlen(VALID_PASSWORD), 0);

                            printf("\nNew credentials: %s %s\n", p->element.username, buffer);
                            strcpy(p->element.password, buffer);
                            writeFile(list);

                            memset(buffer, 0, sizeof(buffer));
                            recv(new_socket, buffer, sizeof(buffer), 0);
                            if(strlen(received_alpha) == 0)
                                send(new_socket, EMPTY_STRING, strlen(EMPTY_STRING), 0);
                            else
                                send(new_socket, received_alpha, strlen(received_alpha), 0);
                            
                            memset(buffer, 0, sizeof(buffer));
                            recv(new_socket, buffer, sizeof(buffer), 0);
                            if(strlen(received_num) == 0)
                                send(new_socket, EMPTY_STRING, strlen(EMPTY_STRING), 0);
                            else
                                send(new_socket, received_num, strlen(received_num), 0);
                        }
                        else
                        {
                            printf("\nPassword is invalid\n");
                            send(new_socket, INVALID_PASSWORD, strlen(INVALID_PASSWORD), 0);
                        }
                    }
                    break;
                case 2:
                    printf("\nUser status is new.\n");
                    send(new_socket, USER_NOT_ACTIVATED, strlen(USER_FOUND), 0);
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    return 0;
}
