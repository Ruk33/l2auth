#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"
#include "../server_packet/enter_world.h"
#include "../character.h"
#include "enter_world.h"

void client_request_enter_world(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_ENTER_WORLD_FULL_SIZE] = {0};
        character_t *character = storage_character_active_from_session(
                character_storage,
                session
        );

        if (!character)
        {
                printf("Warning: entering world with missing character.\n");
                printf("Ignoring request\n");
                return;
        }

        session_entered_world(session);

        server_packet_enter_world(response, character);
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
