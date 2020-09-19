#ifndef CREATE_CHAR_REQUEST_H
#define CREATE_CHAR_REQUEST_H

#include <core/l2_raw_packet.h>
#include <core/l2_string.h>
#include "../../client.h"

struct CreateCharRequest {
        l2_string name[20]; // Hardcoded, not sure if this is right limit
        int race;
        int sex;
        int class_id;
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

struct CreateCharRequest *create_char_request
(struct Client *client, l2_raw_packet *packet);

#endif
