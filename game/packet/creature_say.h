#ifndef L2AUTH_GAME_PACKET_CREATURE_SAY_H
#define L2AUTH_GAME_PACKET_CREATURE_SAY_H

#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/l2_string.h>

l2_packet* game_packet_creature_say
(
        struct L2Client *client,
        l2_string* message
);

#endif
