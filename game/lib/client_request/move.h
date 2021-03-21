#ifndef LIB_CLIENT_REQUEST_MOVE_H
#define LIB_CLIENT_REQUEST_MOVE_H

#include <headers.h>
#include <session.h>
#include <character.h>

typedef struct {
        vec3_t position;
} client_request_move_t;

void client_request_move(client_request_move_t *dest, packet *request);

void client_request_move_as_packet(packet *dest, client_request_move_t *src);

#endif
