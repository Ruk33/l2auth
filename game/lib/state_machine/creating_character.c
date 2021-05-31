#include <request.h>
#include <session.h>
#include <character.h>
#include <util/session_crypt.h>
#include <db/session.h>
#include <db/player.h>
#include <client_request/create_char.h>
#include <client_packet/type.h>
#include <server_packet/create_char.h>
#include "auth_request.h"
#include "character_selection.h"
#include "creating_character.h"

#define create_char_packet_size \
        PACKET_SAFE_FULL_SIZE(server_packet_create_char_t)

static void create_character(request_t *req)
{
        session_t session = { 0 };

        client_request_create_char_t parsed_req = { 0 };

        packet response[create_char_packet_size] = { 0 };
        packet request_characters_packet[8]      = { 0 };

        request_t       request_characters = { 0 };
        character_t     new_character      = { 0 };
        session_state_t char_selection     = 0;
        size_t          name_size          = 0;

        assert_valid_request(req);

        char_selection = CHARACTER_SELECTION;
        name_size      = sizeof(new_character.name);

        client_request_create_char(&parsed_req, req->packet);
        util_session_update_state(req->storage, req->socket, char_selection);

        /*
         * Prepare the new character to be stored.
         * We may want to refactor this out to
         * another function, not sure yet.
         */
        // new_character.session = request->session;
        new_character.id = rand();
        l2_string_to_char(new_character.name, parsed_req.name, name_size);
        new_character.active   = 1;
        new_character.hp       = 42;
        new_character.mp       = 42;
        new_character.max_hp   = 42;
        new_character.max_mp   = 33;
        new_character.level    = 1;
        new_character.sex      = parsed_req.sex;
        new_character.race_id  = parsed_req.race;
        new_character.class_id = parsed_req._class;

        db_session_get(req->storage, &session, req->socket);
        db_player_add(req->storage, session.username, &new_character);

        /*
         * Send acknowledgement to the client
         * that the character was created successfully.
         */
        server_packet_create_char(response);
        util_session_encrypt_packet(
                req->storage,
                req->socket,
                response,
                response,
                (size_t) packet_get_size(response));
        request_send_response(req, response, packet_get_size(response));

        /**
         * TODO: resend packet using gs request.
         */

        /*
         * Create a dummy request to refetch all
         * characters from a session and send
         * them over to the client so the new
         * character is visible.
         */
        packet_build(
                request_characters_packet,
                (unsigned char) CLIENT_PACKET_TYPE_AUTH_REQUEST,
                NULL,
                0);

        memcpy(&request_characters, req, sizeof(request_characters));
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
                printf("Packet %02X can't be handled by state_machine_creating_character.\n",
                       type);
                printf("Delegating to state_machine_character_selection\n");
                state_machine_character_selection(request);
                break;
        }
}
