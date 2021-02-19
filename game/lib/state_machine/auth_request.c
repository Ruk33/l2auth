#include <request.h>
#include <storage/character.h>
#include <server_packet/auth_login.h>
#include <client_request/auth_request.h>
#include <client_packet/type.h>
#include "auth_request.h"

/**
 * Send all the characters associated to a session.
 * This is used when the user just logged into
 * the game server.
 */
static void send_playable_characters(request_t *request)
{
        struct List *characters = NULL;
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_auth_login_t)] = {0};

        assert_valid_request(request);

        characters = storage_character_all_from_session(&request->storage->character_storage, request->session);

        server_packet_auth_login(response, request->session->playOK1, characters);
        session_encrypt_packet(request->session, response, response, (size_t) packet_get_size(response));
        request->host->send_response(request->session->socket, response, (size_t) packet_get_size(response));
}

/**
 * Handle auth request made by client.
 * This request is made when the user is transitioning
 * from the login to the game server.
 */
static void auth_request(request_t *request, int update_session)
{
        client_request_auth_t parsed_request = {0};
        // Hardcoded, not sure if this is the correct maximum
        char username[28] = {0};

        if (update_session) {
                assert_valid_request(request);

                client_request_auth_request(&parsed_request, request->packet);
                l2_string_to_char(username, parsed_request.username, sizeof(username));

                session_update(request->session, username, strlen(username) + 1, parsed_request.loginOK1, parsed_request.loginOK2, parsed_request.playOK1, parsed_request.playOK2);
        }

        session_update_state(request->session, CHARACTER_SELECTION);
        send_playable_characters(request);
}

void state_machine_auth_request(request_t *request, int update_session)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_AUTH_REQUEST:
                auth_request(request, update_session);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_auth_request.\n", type);
                break;
        }
}
