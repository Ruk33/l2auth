#ifndef NPC_INFO_HANDLER_H
#define NPC_INFO_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void npc_info_handler(struct Client *client, l2_raw_packet *packet);

#endif
