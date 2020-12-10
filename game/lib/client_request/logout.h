#ifndef LIB_CLIENT_REQUEST_LOGOUT_H
#define LIB_CLIENT_REQUEST_LOGOUT_H

#include "../headers.h"
#include "../session.h"

void client_request_logout(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
