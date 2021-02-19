#ifndef LIB_CLIENT_REQUEST_SELECT_CHARACTER_H
#define LIB_CLIENT_REQUEST_SELECT_CHARACTER_H

#include <headers.h>
#include <session.h>
#include <storage/character.h>

struct ClientRequestSelectCharacter {
        unsigned int index;
};

typedef struct ClientRequestSelectCharacter client_request_select_character_t;

/**
 * Client makes this request when the user
 * selects a character to play with.
 * Parse the raw request and store it on dest.
 */
void client_request_select_character(client_request_select_character_t *dest, packet *request);

#endif
