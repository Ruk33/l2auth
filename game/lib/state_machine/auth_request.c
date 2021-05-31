#include <headers.h>
#include <request.h>
#include <character.h>
#include <util/session_crypt.h>
#include <db/player.h>
#include <db/character.h>
#include <db/session.h>
#include <server_packet/auth_login.h>
#include <client_request/auth_request.h>
#include <client_packet/type.h>
#include <session.h>
#include "auth_request.h"

#define auth_login_packet_size PACKET_SAFE_FULL_SIZE(server_packet_auth_login_t)

/**
 * Send all the characters associated to a session.
 * This is used when the user just logged into
 * the game server.
 */
static void send_playable_characters(request_t *request)
{
        session_t session = { 0 };

        /**
         * TODO: Refactor hardcoded max amount of chars.
         */
        character_t chars[5]    = { 0 };
        size_t      chars_count = 0;

        packet response[auth_login_packet_size] = { 0 };

        assert_valid_request(request);

        db_session_get(request->storage, &session, request->socket);

        /**
         * TODO: Get all players from session (account)
         */
        // character_count = db_player_all_from_account(
        //         request->storage,
        //         request->session->username,
        //         characters,
        //         sizeof(characters));

        server_packet_auth_login(response, session.playOK1, chars, chars_count);
        util_session_encrypt_packet(
                request->storage,
                request->socket,
                response,
                response,
                (size_t) packet_get_size(response));
        request_send_response(request, response, packet_get_size(response));
}

/**
 * Handle auth request made by client.
 * This request is made when the user is transitioning
 * from the login to the game server.
 */
static void auth_request(request_t *req, int update_session)
{
        client_request_auth_t parsed_req = { 0 };

        /**
         * TODO: Refactor hardcoded value. Not sure if this is
         * the correct limit.
         */
        char acc[28] = { 0 };

        session_state_t char_select_state = 0;

        if (update_session) {
                assert_valid_request(req);

                client_request_auth_request(&parsed_req, req->packet);
                l2_string_to_char(acc, parsed_req.username, sizeof(acc));
                util_session_update_account(req->storage, req->socket, acc);

                /**
                 * TODO: Not sure if loginok and playok are needed.
                 * If not, simply remove this commented code.
                 */
                // session_update(
                //         request->session,
                //         username,
                //         strlen(username) + 1,
                //         parsed_request.loginOK1,
                //         parsed_request.loginOK2,
                //         parsed_request.playOK1,
                //         parsed_request.playOK2);
        }

        char_select_state = CHARACTER_SELECTION;
        util_session_update_state(req->storage, req->socket, char_select_state);

        /**
         * TODO: Send this packet as a gs_request.
         */
        // printf("SEND AS GS PACKET!!!\n");
        send_playable_characters(req);
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
                printf("Packet %02X can't be handled by state_machine_auth_request.\n",
                       type);
                break;
        }
}
