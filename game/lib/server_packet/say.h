#ifndef LIB_SERVER_PACKET_SAY_H
#define LIB_SERVER_PACKET_SAY_H

#include "../headers.h"
#include "../character.h"

enum ServerPacketSayMsgType {
        SAY_MSG_TYPE_NORMAL,
        SAY_MSG_TYPE_SHOUT,
        SAY_MSG_TYPE_WHISPER,
        SAY_MSG_TYPE_PARTY,
        SAY_MSG_TYPE_CLAN
};

struct ServerPacketSay {
        int sender_obj_id;
        enum ServerPacketSayMsgType msg_type;
        l2_string sender_name[28];
        l2_string msg[256];
};

#define SERVER_PACKET_SAY_SIZE sizeof(struct ServerPacketSay)
#define SERVER_PACKET_SAY_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_SAY_SIZE

void server_packet_say(
        packet *dest,
        character_t *character,
        enum ServerPacketSayMsgType msg_type,
        char *msg,
        size_t msg_size
);

#endif
