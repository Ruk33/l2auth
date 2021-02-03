#include <headers.h>
#include "auth_request.h"

void client_request_auth_request(client_request_auth_t *dest, packet *request)
{
        assert(dest);
        assert(request);

        byte_t *p = NULL;

        p = packet_body(request);

        dest->username = p;
        p += l2_string_bytes(dest->username);

        BYTE_READ_VAL(dest->playOK2, p);
        BYTE_READ_VAL(dest->playOK1, p);
        BYTE_READ_VAL(dest->loginOK1, p);
        BYTE_READ_VAL(dest->loginOK2, p);
}
