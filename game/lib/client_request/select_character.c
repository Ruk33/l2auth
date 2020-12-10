#include "../headers.h"
#include "../session.h"
#include "../storage/character.h"
#include "../server_packet/select_character.h"
#include "../character.h"
#include "select_character.h"

static void update_selected_character_index_from_packet(
        session_t *session,
        packet *request
)
{
        assert(session);
        assert(request);

        byte *p = packet_body(request);
        BYTE_READ_VAL(session->selected_character_index, p);
        printf("Selected characted updated to %d.\n", session->selected_character_index);
}

void client_request_select_character(
        int client,
        packet *request,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(request);
        assert(session);
        assert(character_storage);
        assert(send_response);

        packet response[SERVER_PACKET_SELECT_CHARACTER_FULL_SIZE] = {0};
        character_t *selected_character = NULL;

        update_selected_character_index_from_packet(session, request);

        selected_character = storage_character_active_from_session(
                character_storage,
                session
        );

        if (!selected_character)
        {
                printf("Warning, selected character not found.\n");
                printf("Ignoring request.\n");
                return;
        }

        printf("Selected character: %s.\n", selected_character->name);

        server_packet_select_character(
                response,
                selected_character,
                session->playOK1
        );
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
