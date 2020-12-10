#include "../headers.h"
#include "../server_packet/new_char.h"
#include "../session.h"
#include "new_char.h"

void client_request_new_char(
        int client,
        session_t *session,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_NEW_CHAR_FULL_SIZE] = {0};

        server_packet_new_char(response);
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
