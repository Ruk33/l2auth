#ifndef LIB_SERVER_PACKET_MOVE_H
#define LIB_SERVER_PACKET_MOVE_H

#include "../headers.h"
#include "../character.h"

struct ServerPacketMove {
        int obj_id;
        vec3_t new_position;
        vec3_t old_position;
};

#define SERVER_PACKET_MOVE_SIZE sizeof(struct ServerPacketMove)
#define SERVER_PACKET_MOVE_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_MOVE_SIZE

void server_packet_move(packet *dest, character_t *character, vec3_t *position);

#endif
