#ifndef LIB_CLIENT_REQUEST_QUEST_LIST_H
#define LIB_CLIENT_REQUEST_QUEST_LIST_H

#include "../headers.h"
#include "../session.h"

void client_request_quest_list(
        int client,
        session_t *session,
        host_send_response_cb send_response
);

#endif
