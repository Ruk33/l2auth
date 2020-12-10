#ifndef LIB_CLIENT_REQUEST_RESTART_H
#define LIB_CLIENT_REQUEST_RESTART_H

#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"

void client_request_restart(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
);

#endif
