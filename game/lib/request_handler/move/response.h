#ifndef MOVE_RESPONSE_H
#define MOVE_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"
#include "../../dto/vec3.h"

l2_packet *move_response(struct Client *client, struct Vec3 prev_location, struct Vec3 new_location);

#endif
