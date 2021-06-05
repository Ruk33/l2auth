#ifndef INCLUDE_GS_PACKET_NEW_CHAR_H
#define INCLUDE_GS_PACKET_NEW_CHAR_H

#include "packet.h"
#include "gs_character_template.h"

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
} gs_packet_new_char_template_t;

typedef struct {
        int count;
        gs_packet_new_char_template_t templates[10];
} gs_packet_new_char_t;

void gs_packet_new_char_add_template(
        gs_packet_new_char_t *dest,
        gs_character_template_t *src);

void gs_packet_new_char_pack(packet_t *dest, gs_packet_new_char_t *src);

#endif
