#ifndef SERVER_PACKET_PLAY_OK_H
#define SERVER_PACKET_PLAY_OK_H

#include "../storage/game_server.h"

struct ServerPacketPlayOk {
        int playOK1;
        int playOK2;
};

#define SERVER_PACKET_PLAY_OK_SIZE sizeof(struct ServerPacketPlayOk)
#define SERVER_PACKET_PLAY_OK_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_PLAY_OK_SIZE
#define SERVER_PACKET_PLAY_OK_FULL_SIZE_ENCRYPTED sizeof(short) + ((SERVER_PACKET_PLAY_OK_SIZE + sizeof(char) + 7) & (~7))

void server_packet_play_ok(unsigned char *dest, int playOK1, int playOK2);

#endif
