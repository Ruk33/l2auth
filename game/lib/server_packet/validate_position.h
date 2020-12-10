#ifndef LIB_SERVER_PACKET_VALIDATE_POSITION_H
#define LIB_SERVER_PACKET_VALIDATE_POSITION_H

#include "../headers.h"
#include "../character.h"

struct ServerPacketValidatePosition {
        int obj_id;
        int x;
        int y;
        int z;
        int heading;
};

#define SERVER_PACKET_VALIDATE_POSITION_SIZE sizeof(struct ServerPacketValidatePosition)
#define SERVER_PACKET_VALIDATE_POSITION_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_VALIDATE_POSITION_SIZE

void server_packet_validate_position(packet *dest, character_t *character, int heading);

#endif
