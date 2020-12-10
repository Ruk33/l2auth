#ifndef LIB_CLIENT_REQUEST_MOVE_H
#define LIB_CLIENT_REQUEST_MOVE_H

#include "../headers.h"
#include "../session.h"
#include "../character.h"

struct ClientRequestMove {
        vec3_t position;
};

void client_request_move(
        int client,
        packet *request,
        session_t *session,
        character_t *character,
        host_send_response_cb send_response
);

#endif
