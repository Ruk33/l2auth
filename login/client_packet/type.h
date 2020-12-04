#ifndef CLIENT_PACKET_TYPE_H
#define CLIENT_PACKET_TYPE_H

/**
 * List of packet types
 * sent by the client.
 */
enum ClientPacketType {
        CLIENT_PACKET_TYPE_AUTH_LOGIN = 0x00,
        CLIENT_PACKET_TYPE_LOGIN_SERVER = 0x02,
        CLIENT_PACKET_TYPE_REQUEST_SERVER_LIST = 0x05,
        CLIENT_PACKET_TYPE_GG_AUTH = 0x07
};

#endif
