#ifndef INCLUDE_CHARACTER_H
#define INCLUDE_CHARACTER_H

#include "packet_create_char_request.h"

typedef struct {
        char name[32];
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
} character_t;

// Fill character d from client request (made when
// creating a new character).
void character_from_request(character_t *d, packet_create_char_request_t *src);

#endif
