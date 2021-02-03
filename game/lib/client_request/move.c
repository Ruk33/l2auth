#include <headers.h>
#include <session.h>
#include <server_packet/move.h>
#include <character.h>
#include "move.h"

void client_request_move(client_request_move_t *dest, packet *request)
{
        byte_t *p = NULL;

        assert(dest);
        assert(request);

        p = packet_body(request);

        BYTE_READ_VAL(dest->position.x, p);
        BYTE_READ_VAL(dest->position.y, p);
        BYTE_READ_VAL(dest->position.z, p);
}
