#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const char * const REQUEST_PASSWORD = "Requesting password";
const char * const USER_FOUND = "User found";
const char * const USER_NOT_FOUND = "User not found";
const char * const VALID_PASSWORD = "Valid password";
const char * const INVALID_PASSWORD = "Invalid password";
const char * const USER_BLOCKED = "User blocked";
const char * const NMSG_INVALID_INPUT = "Invalid input";
const char * const NMSG_SENT = "Message sent to Client / Server";
const char * const USER_NOT_ACTIVATED = "User not activated";
const char * const REQUEST_MSG = "Requesting message";
const char * const EMPTY_STRING = "EMPTY STRING";

bool is_valid_ip(const char *ip) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0;
}

bool is_valid_port(const char *port) {
    int p = atoi(port);
    return p > 0 && p < 65536;
}
