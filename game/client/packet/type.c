#ifndef L2AUTH_LOGIN_GAME_PACKET_CLIENT_TYPE
#define L2AUTH_LOGIN_GAME_PACKET_CLIENT_TYPE

enum game_client_packet_type
{
        PROTOCOL_VERSION = 0x00,
        ENTER_WORLD = 0x03,
        AUTH_REQUEST = 0x08,
        NEW_CHAR = 0x0e,
        CREATE_CHAR = 0x0b,
        SELECT_CHAR = 0x0d,
        REQUEST_SS_BSPS = 0xd0,
        REQUEST_QUEST_LIST = 0x63,
        RESTART = 0x46,
};

#endif
