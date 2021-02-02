#ifndef LIB_CLIENT_REQUEST_SAY_H
#define LIB_CLIENT_REQUEST_SAY_H

#include <headers.h>
#include <session.h>
#include <character.h>

struct ClientRequestSay {
        l2_string_t *message;
        l2_string_type_t type;
};

typedef struct ClientRequestSay client_request_say_t;

/**
 * Parse a character's send message request
 * and store it on dest.
 */
void client_request_say(client_request_say_t *dest, packet *request);

#endif
