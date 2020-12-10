#ifndef LIB_CLIENT_REQUEST_VALIDATE_POSITION_H
#define LIB_CLIENT_REQUEST_VALIDATE_POSITION_H

#include "../headers.h"
#include "../session.h"
#include "../character.h"

struct ClientRequestValidatePosition {
        vec3_t position;
        int heading;
};

void client_request_validate_position(
        int client,
        packet *request,
        session_t *session,
        character_t *character,
        host_send_response_cb send_response
);

#endif
