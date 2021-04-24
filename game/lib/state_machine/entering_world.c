#include <request.h>
#include <util/session_crypt.h>
#include <server_packet/d0.h>
#include <server_packet/enter_world.h>
#include <server_packet/quest_list.h>
#include <client_packet/type.h>
#include "in_world.h"
#include "entering_world.h"

static void d0(request_t *request)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_d0_t)] = { 0 };

        assert_valid_request(request);

        server_packet_d0(response);
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

static void quest_list(request_t *request)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_quest_list_t)] = {
                0
        };

        assert_valid_request(request);

        server_packet_quest_list(response);
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

        session_update_state(request->session, ENTERING_WORLD);
}

static void enter_world(request_t *request)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_enter_world_t)] = {
                0
        };
        character_t *character = NULL;

        assert_valid_request(request);

        character = NULL;

        if (!character) {
                printf("Warning: entering world with missing character.\n");
                printf("Ignoring request\n");
                return;
        }

        server_packet_enter_world(response, character);
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

        session_update_state(request->session, IN_WORLD);

        /*
         * This is being executed so the first spawn
         * state is being handled (it notifies other players
         * of the presence of the character).
         *
         * After that, the caracter's state will be idle.
         */
        state_machine_in_world(request);
}

void state_machine_entering_world(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_REQUEST_AUTO_SS_BSPS:
                d0(request);
                break;
        case CLIENT_PACKET_TYPE_REQUEST_QUEST_LIST:
                quest_list(request);
                break;
        case CLIENT_PACKET_TYPE_ENTER_WORLD:
                enter_world(request);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_entering_world.\n",
                       type);
                break;
        }
}
