#ifndef LIB_CLIENT_REQUEST_AUTH_REQUEST_H
#define LIB_CLIENT_REQUEST_AUTH_REQUEST_H

#include <headers.h>
#include <storage/memory/session.h>
#include <storage/memory/character.h>

struct ClientRequestAuth {
        l2_string_t *username;
        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
};

typedef struct ClientRequestAuth client_request_auth_t;

/**
 * Client makes this request when accessing a game server.
 * Parse the raw request and store it on dest.
 */
void client_request_auth_request(client_request_auth_t *dest, packet *request);

#endif
