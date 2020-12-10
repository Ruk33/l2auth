#ifndef LIB_CLIENT_REQUEST_ENTER_WORLD_H
#define LIB_CLIENT_REQUEST_ENTER_WORLD_H

#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"

void client_request_enter_world(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
);

#endif
