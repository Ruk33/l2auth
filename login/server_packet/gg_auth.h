#ifndef SERVER_PACKET_GG_AUTH_H
#define SERVER_PACKET_GG_AUTH_H

enum ServerPacketGGAuthResponse {
        SERVER_PACKET_GG_AUTH_RESPONSE_SKIP_GG = 0x0b
};

struct ServerPacketGGAuth {
        enum ServerPacketGGAuthResponse response;
};

#define SERVER_PACKET_GG_AUTH_SIZE sizeof(struct ServerPacketGGAuth)
#define SERVER_PACKET_GG_AUTH_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_GG_AUTH_SIZE
#define SERVER_PACKET_GG_AUTH_FULL_SIZE_ENCRYPTED sizeof(short) + ((SERVER_PACKET_GG_AUTH_SIZE + sizeof(char) + 7) & (~7))

void server_packet_gg_auth(unsigned char *dest, enum ServerPacketGGAuthResponse response);

#endif
