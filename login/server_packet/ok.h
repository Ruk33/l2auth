#ifndef SERVER_PACKET_OK_H
#define SERVER_PACKET_OK_H

struct ServerPacketOk {
        int loginOK1;
        int loginOK2;
        unsigned char after_key[39];
};

#define SERVER_PACKET_OK_SIZE sizeof(struct ServerPacketOk)
#define SERVER_PACKET_OK_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_OK_SIZE 
#define SERVER_PACKET_OK_FULL_SIZE_ENCRYPTED sizeof(short) + ((SERVER_PACKET_OK_SIZE + sizeof(char) + 7) & (~7))

void server_packet_ok(unsigned char *dest, int loginOK1, int loginOK2);

#endif
