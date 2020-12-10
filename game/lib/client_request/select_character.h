#ifndef LIB_CLIENT_REQUEST_SELECT_CHARACTER_H
#define LIB_CLIENT_REQUEST_SELECT_CHARACTER_H

#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"

void client_request_select_character(
        int client,
        packet *request,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
);

#endif
