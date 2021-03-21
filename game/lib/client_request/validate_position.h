#ifndef LIB_CLIENT_REQUEST_VALIDATE_POSITION_H
#define LIB_CLIENT_REQUEST_VALIDATE_POSITION_H

#include <headers.h>
#include <session.h>
#include <character.h>

typedef struct {
        position_t position;
        int        heading;
} client_request_validate_position_t;

void client_request_validate_position(
        client_request_validate_position_t *dest,
        packet *                            request);

#endif
