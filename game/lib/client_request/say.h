#ifndef LIB_CLIENT_REQUEST_SAY_H
#define LIB_CLIENT_REQUEST_SAY_H

#include "../headers.h"
#include "../session.h"
#include "../character.h"

void client_request_say(
        int client,
        packet *request,
        session_t *session,
        character_t *character,
        host_send_response_cb send_response
);

#endif
