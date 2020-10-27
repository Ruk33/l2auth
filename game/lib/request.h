#ifndef REQUEST_H
#define REQUEST_H

#include <core/l2_raw_packet.h>
#include "world_manager.h"
#include "client.h"

struct Request
{
        struct WorldManager *world_state;
        struct Client *client;
        l2_raw_packet *packet;
};

#endif
