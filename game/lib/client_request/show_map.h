#ifndef LIB_CLIENT_REQUEST_SHOW_MAP_H
#define LIB_CLIENT_REQUEST_SHOW_MAP_H

#include "../headers.h"
#include "../session.h"

void client_request_show_map(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
