#include "../headers.h"
#include "../session.h"
#include "../server_packet/quest_list.h"
#include "quest_list.h"

void client_request_quest_list(
        int client,
        session_t *session,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_QUEST_LIST_FULL_SIZE] = {0};

        server_packet_quest_list(response);
        session_encrypt_packet(
                session,
                response,
                response,
                (size_t) packet_get_size(response)
        );
        send_response(
                client,
                response,
                (size_t) packet_get_size(response)
        );
}
