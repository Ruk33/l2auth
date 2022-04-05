#ifndef TYPES_H
#define TYPES_H

#include "client.h"

struct state {
    struct client clients[32];
};

#endif