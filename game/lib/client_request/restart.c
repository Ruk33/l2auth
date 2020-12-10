#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"
#include "../server_packet/restart.h"
#include "../client_request/auth_request.h"
#include "restart.h"

void client_request_restart(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_RESTART_FULL_SIZE] = {0};

        server_packet_restart(response);
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

        client_request_auth_request_from_session(
                client,
                session,
                character_storage,
                send_response
        );

        session_leaved_world(session);
}
