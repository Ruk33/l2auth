#ifndef LIB_CLIENT_REQUEST_PROTOCOL_VERSION_H
#define LIB_CLIENT_REQUEST_PROTOCOL_VERSION_H

#include "../headers.h"

/**
 * First request made by client
 * after selecting a game server.
 */
void client_request_protocol_version(
        int client,
        host_send_response_cb send_response
);

#endif
