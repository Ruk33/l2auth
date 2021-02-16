#include <request.h>
#include <client_request/create_char.h>
#include <client_packet/type.h>
#include <server_packet/create_char.h>
#include "auth_request.h"
#include "character_selection.h"
#include "creating_character.h"

static void create_character(request_t *request)
{
        client_request_create_char_t parsed_request = {0};
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_create_char_t)] = {0};
        character_t new_character = {0};

        packet request_characters_packet[8] = {0};
        request_t request_characters = {0};

        assert_valid_request(request);

        client_request_create_char(&parsed_request, request->packet);
        session_update_state(request->session, CHARACTER_SELECTION);

        /*
         * Prepare the new character to be stored.
         * We may want to refactor this out to
         * another function, not sure yet.
         */
        new_character.session = request->session;
        new_character.id = rand();
        l2_string_to_char(new_character.name, parsed_request.name, sizeof(new_character.name));
        new_character.active = 1,
        new_character.hp = 42;
        new_character.mp = 42;
        new_character.max_hp = 42;
        new_character.max_mp = 33;
        new_character.level = 1;
        new_character.sex = parsed_request.sex;
        new_character.race_id = parsed_request.race;
        new_character.class_id = parsed_request._class;

        storage_character_add(&request->storage->character_storage, request->session->username, strlen(request->session->username) + 1, &new_character);

        /*
         * Send acknowledgement to the client
         * that the character was created successfully.
         */
        server_packet_create_char(response);
        session_encrypt_packet(request->session, response, response, (size_t) packet_get_size(response));
        request->host->send_response(request->session->socket, response, (size_t) packet_get_size(response));

        /*
         * Create a dummy request to refetch all
         * characters from a session and send
         * them over to the client so the new
         * character is visible.
         */
        packet_build(request_characters_packet, (unsigned char) CLIENT_PACKET_TYPE_AUTH_REQUEST, NULL, 0);

        memcpy(&request_characters, request, sizeof(request_characters));
        request_characters.packet = request_characters_packet;

        state_machine_auth_request(&request_characters, 0);
}

void state_machine_creating_character(request_t *request)
{
        packet_type type = 0;

        assert_valid_request(request);
        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_CREATE_CHAR:
                create_character(request);
                break;
        default:
                /*
                 * When creating a new character, the user can go
                 * back to the character selection screen and no
                 * packet will be sent. This is why we also need to
                 * accept states from character selection.
                 */
                printf("Packet %02X can't be handled by state_machine_creating_character.\n", type);
                printf("Delegating to state_machine_character_selection\n");
                state_machine_character_selection(request);
                break;
        }
}
