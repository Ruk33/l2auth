#ifndef REQUEST_H
#define REQUEST_H

#include <core/l2_raw_packet.h>
#include "world_state.h"
#include "client.h"

struct Request
{
        struct WorldState *world_state;
        struct Client *client;
        l2_raw_packet *packet;
};

#endif
