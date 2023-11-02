#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "lib/util.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s [SERVER_IP] [SERVER_PORT]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!is_valid_ip(argv[1]))
    {
        printf("Invalid server IP address: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!is_valid_port(argv[2]))
    {
        printf("Invalid server port: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(server_port),
    };
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    socklen_t server_len = sizeof(server_addr);
    char message[1024] = {0};
    printf("Enter username: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // remove newline character
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, server_len) < 0)
    {
        perror("send failed");
        exit(EXIT_FAILURE);
    }
    memset(message, 0, sizeof(message));

    char buffer[1024] = {0};
    while (true)
    {
        char message[1024] = {0};

        // Receive message from server
        memset(buffer, 0, sizeof(buffer));
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_len) < 0)
        {
            perror("receive failed");
            exit(EXIT_FAILURE);
        }
        printf("Server response: %s\n", buffer);

        // Send response to server
        memset(message, 0, sizeof(message));
        if (strcmp(buffer, REQUEST_PASSWORD) == 0)
        {
            printf("\nEnter password: ");
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = '\0';
        }
        else if (strcmp(buffer, USER_NOT_FOUND) == 0)
        {
            printf("\nUser not found\n");
            close(sockfd);
            return 0;
        }
        else if (strcmp(buffer, USER_FOUND) == 0)
        {
            printf("\nWelcome back\n");
            memset(message, 0, sizeof(message));
            while (1)
            {
                printf("\nChange password:");
                fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = '\0';
                if (strcmp(message, "") == 0)
                {
                    printf("\nEmpty String!\n");
                    continue;
                }
                sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, server_len);
                if (strcmp(message, "bye") == 0)
                {
                    printf("\nGoodbye\n");
                    break;
                }
                else
                {
                    memset(buffer, 0, sizeof(buffer));
                    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_len);

                    if (strcmp(buffer, VALID_PASSWORD) == 0)
                    {
                        sendto(sockfd, REQUEST_MSG, strlen(REQUEST_MSG), 0, (struct sockaddr *)&server_addr, server_len);
                        memset(buffer, 0, sizeof(buffer));
                        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_len);
                        if (strcmp(buffer, EMPTY_STRING) != 0)
                            printf("\n%s", buffer);
                        sendto(sockfd, REQUEST_MSG, strlen(REQUEST_MSG), 0, (struct sockaddr *)&server_addr, server_len);
                        memset(buffer, 0, sizeof(buffer));
                        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_len);
                        if (strcmp(buffer, EMPTY_STRING) != 0)
                            printf("\n%s\n", buffer);
                    }
                    else if (strcmp(buffer, INVALID_PASSWORD) == 0)
                        printf("\nPassword invalid!");
                    else
                        printf("\n~YOU'VE DONE FUCKED UP!~");
                }
            }
            close(sockfd);
            return 0;
        }
        else if (strcmp(buffer, INVALID_PASSWORD) == 0)
        {
            printf("\nRe-enter password: ");
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = '\0';
        }
        else if (strcmp(buffer, USER_NOT_ACTIVATED) == 0)
        {
            printf("\nAccount is not ready\n");
            close(sockfd);
            return 0;
        }
        else if (strcmp(buffer, USER_BLOCKED) == 0)
        {
            printf("\nAccount is blocked\n");
            close(sockfd);
            return 0;
        }
        if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, server_len) < 0)
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);
    return 0;
}
