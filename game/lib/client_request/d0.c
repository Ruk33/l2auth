#include "../headers.h"
#include "../session.h"
#include "../server_packet/d0.h"
#include "d0.h"

void client_request_d0(
        int client,
        session_t *session,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_D0_FULL_SIZE] = {0};

        server_packet_d0(response);
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
