#ifndef LIB_CLIENT_REQUEST_REQUEST_UPPERCASE_NAME_H
#define LIB_CLIENT_REQUEST_REQUEST_UPPERCASE_NAME_H

#include "../headers.h"
#include "../session.h"

void client_request_REQUEST_LOWERCASE_NAME(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
