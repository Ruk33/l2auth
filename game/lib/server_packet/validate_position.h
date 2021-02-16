#ifndef LIB_SERVER_PACKET_VALIDATE_POSITION_H
#define LIB_SERVER_PACKET_VALIDATE_POSITION_H

#include <headers.h>
#include <character.h>

struct ServerPacketValidatePosition {
        int obj_id;
        int x;
        int y;
        int z;
        int heading;
};

typedef struct ServerPacketValidatePosition server_packet_validate_position_t;

void server_packet_validate_position(packet *dest, character_t *character, int heading);

#endif
