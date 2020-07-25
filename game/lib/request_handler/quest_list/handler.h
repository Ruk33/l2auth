#ifndef QUEST_LIST_HANDLER_H
#define QUEST_LIST_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void quest_list_handler(struct Client *client, l2_raw_packet *packet);

#endif
