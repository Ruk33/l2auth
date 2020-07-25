#ifndef AUTH_LOGIN_NEXT_HANDLER_H
#define AUTH_LOGIN_NEXT_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void auth_login_next_handler(struct Client *client, l2_raw_packet *packet);

#endif
