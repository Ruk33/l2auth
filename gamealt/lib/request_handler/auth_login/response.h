#ifndef AUTH_LOGIN_RESPONSE_H
#define AUTH_LOGIN_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *auth_login_response(struct Client *client);

#endif
