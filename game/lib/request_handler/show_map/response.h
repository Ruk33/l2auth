#ifndef SHOW_MAP_RESPONSE_H
#define SHOW_MAP_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *show_map_response(struct Client *client, int map_id);

#endif
