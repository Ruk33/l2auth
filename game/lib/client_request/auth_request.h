#ifndef LIB_CLIENT_REQUEST_AUTH_REQUEST_H
#define LIB_CLIENT_REQUEST_AUTH_REQUEST_H

#include "../headers.h"
#include "../storage/session.h"
#include "../storage/character.h"

/**
 * Client makes this request
 * when accessing a game server.
 * In this request, we will answer with
 * all the characters saved on the account.
 * On this request, the session keys and
 * username will also be updated.
 */
void client_request_auth_request(
        int client,
        packet *request,
        storage_character_t *character_storage,
        session_t *session,
        host_send_response_cb send_response
);

/**
 * Only send characters from session
 * to the client. Useful when the client
 * restarts from in game or creates a
 * new character.
 */
void client_request_auth_request_from_session(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
);

#endif
