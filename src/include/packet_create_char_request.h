#ifndef INCLUDE_PACKET_CREATE_CHAR_REQUEST_H
#define INCLUDE_PACKET_CREATE_CHAR_REQUEST_H

#include "packet.h"
#include "l2_string.h"

typedef struct {
        l2_string_t name[28];
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
} packet_create_char_request_t;

void packet_create_char_request_unpack(
        packet_create_char_request_t *dest,
        packet_t *src);

#endif
