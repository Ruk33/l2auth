#ifndef TYPES_H
#define TYPES_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "client.h"

struct state {
    struct client clients[32];
};

#endif