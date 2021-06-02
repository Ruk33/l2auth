#ifndef INCLUDE_PACKET_NEW_CHAR_H
#define INCLUDE_PACKET_NEW_CHAR_H

#include "packet.h"
#include "character_template.h"

typedef struct {
        int separator_0a[6];
        int separator_46[6];
        int race;
        int _class;
        int str;
        int dex;
        int con;
        int _int;
        int wit;
        int men;
} packet_new_char_template_t;

typedef struct {
        int count;

        packet_new_char_template_t templates[10];
} packet_new_char_t;

void packet_new_char_add_template(packet_new_char_t *d, character_template_t *s);

void packet_new_char_pack(packet_t *dest, packet_new_char_t *src);

#endif
