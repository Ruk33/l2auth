#ifndef LIB_SERVER_PACKET_MOVE_H
#define LIB_SERVER_PACKET_MOVE_H

#include <headers.h>
#include <character.h>

struct ServerPacketMove {
        int obj_id;
        vec3_t new_position;
        vec3_t old_position;
};

typedef struct ServerPacketMove server_packet_move_t;

void server_packet_move(packet *dest, character_t *character, vec3_t *position);

#endif
