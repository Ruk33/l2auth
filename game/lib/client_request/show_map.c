#include "../headers.h"
#include "../session.h"
#include "../server_packet/show_map.h"
#include "show_map.h"

void client_request_show_map(
        int client,
        session_t *session,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_SHOW_MAP_FULL_SIZE] = {0};

        server_packet_show_map(response, 1665);
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
