#ifndef VALIDATE_POSITION_RESPONSE_H
#define VALIDATE_POSITION_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"
#include "../../dto/vec3.h"

l2_packet *validate_position_response
(struct Client *client, struct Vec3 location, int heading);

#endif
