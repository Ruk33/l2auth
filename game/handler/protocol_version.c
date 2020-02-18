#ifndef L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_C
#define L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_C

#include <string.h>
#include <log/log.h>
#include <core/l2_packet.c>
#include <game/packet/crypt_init.c>

l2_packet* game_handler_protocol_version(l2_raw_packet* request)
{
    int protocol = 0;
    
    log_info("Handling protocol version packet");

    memcpy(&protocol, request + 3 /* 2 = packet size, 1 = packet type */, sizeof(protocol));
    log_info("Protocol version %d", protocol);
    log_info("Returning crypt init");

    return game_packet_crypt_init();
}

#endif