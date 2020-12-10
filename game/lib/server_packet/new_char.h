#ifndef LIB_SERVER_PACKET_NEW_CHAR_H
#define LIB_SERVER_PACKET_NEW_CHAR_H

#include "../headers.h"

struct ServerPacketNewCharCharTemplate {
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
};

struct ServerPacketNewChar {
        int char_templates_count;
        struct ServerPacketNewCharCharTemplate char_templates[10];
};

#define SERVER_PACKET_NEW_CHAR_SIZE sizeof(struct ServerPacketNewChar)
#define SERVER_PACKET_NEW_CHAR_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_NEW_CHAR_SIZE

void server_packet_new_char(packet *dest);

#endif
