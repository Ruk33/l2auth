#ifndef LIB_CLIENT_REQUEST_CREATE_CHAR_H
#define LIB_CLIENT_REQUEST_CREATE_CHAR_H

#include "../headers.h"
#include "../storage/session.h"
#include "../storage/character.h"

struct ClientRequestCreateChar {
        // Hardcoded, not sure if this is right limit
        char name[20];
        int race;
        int sex;
        int _class;
        int _int;
        int str;
        int con;
        int men;
        int dex;
        int wit;
        int hair_style;
        int hair_color;
        int face;
};

/**
 * Client performs this request when
 * finishing creating a new character.
 * The new character information will
 * be sent, meaning, race, name, etc.
 */
void client_request_create_char(
        int client,
        packet *request,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
);

#endif
