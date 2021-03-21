#ifndef LIB_CLIENT_REQUEST_CREATE_CHAR_H
#define LIB_CLIENT_REQUEST_CREATE_CHAR_H

#include <headers.h>
#include <storage/session.h>
#include <storage/character.h>

typedef struct {
        l2_string_t *name;
        int          race;
        int          sex;
        int          _class;
        int          _int;
        int          str;
        int          con;
        int          men;
        int          dex;
        int          wit;
        int          hair_style;
        int          hair_color;
        int          face;
} client_request_create_char_t;

/**
 * Client performs this request when
 * finishing creating a new character.
 * The new character information will
 * be sent, meaning, race, name, etc.
 */
void client_request_create_char(
        client_request_create_char_t *dest,
        packet *                      request);

#endif
