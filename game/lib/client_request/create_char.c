#include <headers.h>
#include <storage/session.h>
#include <storage/character.h>
#include <server_packet/create_char.h>
#include <server_packet/auth_login.h>
#include <client_request/auth_request.h>
#include <character.h>
#include "create_char.h"

void client_request_create_char(client_request_create_char_t *dest, packet *request)
{
        byte_t *p = NULL;

        assert(dest);
        assert(request);

        p = packet_body(request);

        dest->name = p;
        p += l2_string_bytes(p);

        BYTE_READ_VAL(dest->race, p);
        BYTE_READ_VAL(dest->sex, p);
        BYTE_READ_VAL(dest->_class, p);
        BYTE_READ_VAL(dest->_int, p);
        BYTE_READ_VAL(dest->str, p);
        BYTE_READ_VAL(dest->con, p);
        BYTE_READ_VAL(dest->men, p);
        BYTE_READ_VAL(dest->dex, p);
        BYTE_READ_VAL(dest->wit, p);
        BYTE_READ_VAL(dest->hair_style, p);
        BYTE_READ_VAL(dest->hair_color, p);
        BYTE_READ_VAL(dest->face, p);
}
