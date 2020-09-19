#ifndef NEW_CHAR_HANDLER_H
#define NEW_CHAR_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void new_char_handler
(struct Client *client, l2_raw_packet *packet);

#endif
