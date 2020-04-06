#ifndef L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_H
#define L2AUTH_GAME_HANDLER_PROTOCOL_VERSION_H

#include <core/l2_packet.h>
#include <game/packet/crypt_init.h>

l2_packet* game_handler_protocol_version(l2_raw_packet* request);

#endif
