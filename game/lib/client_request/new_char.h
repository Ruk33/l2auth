#ifndef LIB_CLIENT_REQUEST_NEW_CHAR_H
#define LIB_CLIENT_REQUEST_NEW_CHAR_H

#include "../headers.h"
#include "../session.h"

/**
 * Client makes this request
 * when starting the creation
 * of a new character.
 */
void client_request_new_char(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
