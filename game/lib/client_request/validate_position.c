#include <headers.h>
#include <session.h>
#include <server_packet/validate_position.h>
#include <character.h>
#include "validate_position.h"

void client_request_validate_position(
        client_request_validate_position_t *dest,
        packet *                            request)
{
        byte_t *p = NULL;

        assert(dest);
        assert(request);

        p = packet_body(request);

        BYTE_READ_VAL(dest->position.x, p);
        BYTE_READ_VAL(dest->position.y, p);
        BYTE_READ_VAL(dest->position.z, p);
        BYTE_READ_VAL(dest->heading, p);
}
