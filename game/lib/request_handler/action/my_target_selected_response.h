#ifndef MY_TARGET_SELECTED_RESPONSE_H
#define MY_TARGET_SELECTED_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *my_target_selected_response
(struct Client *client, int object_id, short color);

#endif
