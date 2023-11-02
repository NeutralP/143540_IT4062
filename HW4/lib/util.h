#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>

bool is_valid_ip(const char *ip);
bool is_valid_port(const char *port);
extern const char * const REQUEST_PASSWORD;
extern const char * const USER_FOUND;
extern const char * const VALID_PASSWORD;
extern const char * const INVALID_PASSWORD;
extern const char * const USER_NOT_FOUND;
extern const char * const USER_BLOCKED;
extern const char * const USER_NOT_ACTIVATED;
extern const char * const REQUEST_MSG;
extern const char * const NMSG_SENT;
extern const char * const NMSG_INVALID_INPUT;
extern const char * const EMPTY_STRING;


#endif // UTIL_H
