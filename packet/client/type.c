#ifndef L2AUTH_PACKET_CLIENT_TYPE_C
#define L2AUTH_PACKET_CLIENT_TYPE_C

enum packet_client_type
{
        PACKET_CLIENT_TYPE_REQUEST_AUTH_LOGIN = 0x00,
        PACKET_CLIENT_TYPE_REQUEST_LOGIN_SERVER = 0x02,
        PACKET_CLIENT_TYPE_REQUEST_SERVER_LIST = 0x05,
        PACKET_CLIENT_TYPE_GG_AUTH = 0x07
};

#endif
