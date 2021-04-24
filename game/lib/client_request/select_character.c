#include <headers.h>
#include <session.h>
#include <server_packet/select_character.h>
#include <character.h>
#include "select_character.h"

void client_request_select_character(
        client_request_select_character_t *dest,
        packet *                           request)
{
        byte_t *p = NULL;

        assert(dest);
        assert(request);

        p = packet_body(request);
        BYTE_READ_VAL(dest->index, p);
}
