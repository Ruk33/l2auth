#ifndef LIB_CLIENT_REQUEST_D0_H
#define LIB_CLIENT_REQUEST_D0_H

#include "../headers.h"
#include "../session.h"

void client_request_d0(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
