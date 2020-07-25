#ifndef SELECT_CHARACTER_HANDLER_H
#define SELECT_CHARACTER_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void select_character_handler(struct Client *client, l2_raw_packet *packet);

#endif
