#include "../headers.h"
#include "../session.h"
#include "../server_packet/say.h"
#include "say.h"

void client_request_say(client_request_say_t *dest, packet *request)
{
        assert(dest);
        assert(request);

        dest->message = packet_body(request);
        request += l2_string_bytes(dest->message);
        BYTE_READ_VAL(dest->type, request);
}
