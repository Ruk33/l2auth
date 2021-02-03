#ifndef LIB_CLIENT_REQUEST_MOVE_H
#define LIB_CLIENT_REQUEST_MOVE_H

#include <headers.h>
#include <session.h>
#include <character.h>

struct ClientRequestMove {
        vec3_t position;
};

typedef struct ClientRequestMove client_request_move_t;

void client_request_move(client_request_move_t *dest, packet *request);

#endif
