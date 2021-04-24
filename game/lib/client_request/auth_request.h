#ifndef LIB_CLIENT_REQUEST_AUTH_REQUEST_H
#define LIB_CLIENT_REQUEST_AUTH_REQUEST_H

#include <headers.h>

typedef struct {
        l2_string_t *username;
        int          playOK1;
        int          playOK2;
        int          loginOK1;
        int          loginOK2;
} client_request_auth_t;

/**
 * Client makes this request when accessing a game server.
 * Parse the raw request and store it on dest.
 */
void client_request_auth_request(client_request_auth_t *dest, packet *request);

#endif
