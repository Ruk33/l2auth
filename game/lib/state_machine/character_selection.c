#include <request.h>
#include <util/session_crypt.h>
#include <db/player.h>
#include <server_packet/select_character.h>
#include <server_packet/new_char.h>
#include <client_request/select_character.h>
#include <client_packet/type.h>
#include "character_selection.h"

/**
 * The user is in character selection and
 * clicks on create new character.
 * This takes the user to the character creation
 * screen.
 */
static void new_character(request_t *request)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_new_char_t)] = { 0 };

        assert_valid_request(request);

        server_packet_new_char(response);
        util_session_encrypt_packet(
                request->storage,
                request->session->socket,
                response,
                response,
                (size_t) packet_get_size(response));
        request->host->send_response(
                request->session->socket,
                response,
                (size_t) packet_get_size(response));

        session_update_state(request->session, CREATING_CHARACTER);
}

/**
 * The user selects a character to play with.
 * This initiates the process of entering the world.
 */
static void select_character(request_t *request)
{
        client_request_select_character_t parsed_request = { 0 };

        character_t selected_character = { 0 };
        int         character_found    = 0;

        packet response[PACKET_SAFE_FULL_SIZE(
                server_packet_select_character_t)] = { 0 };

        assert_valid_request(request);

        client_request_select_character(&parsed_request, request->packet);

        db_player_by_index(
                request->storage,
                request->session->username,
                &selected_character,
                (int) parsed_request.index);

        character_found = selected_character.id != 0;

        if (!character_found) {
                printf("Character for session was not found.\n");
                return;
        }

        request->session->selected_character_index = parsed_request.index;

        session_update_state(request->session, ENTERING_WORLD);

        server_packet_select_character(
                response, &selected_character, request->session->playOK1);
        util_session_encrypt_packet(
                request->storage,
                request->session->socket,
                response,
                response,
                (size_t) packet_get_size(response));
        request->host->send_response(
                request->session->socket,
                response,
                (size_t) packet_get_size(response));
}

void state_machine_character_selection(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_NEW_CHAR:
                new_character(request);
                break;
        case CLIENT_PACKET_TYPE_SELECTED_CHAR:
                select_character(request);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_character_selection.\n",
                       type);
                break;
        }
}
