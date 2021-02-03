#include <request.h>
#include <server_packet/d0.h>
#include <server_packet/enter_world.h>
#include <client_request/quest_list.h>
#include <client_packet/type.h>
#include "in_world.h"
#include "entering_world.h"

static void d0(request_t *request)
{
        packet response[SERVER_PACKET_D0_FULL_SIZE] = {0};

        assert_valid_request(request);

        server_packet_d0(response);
        session_encrypt_packet(request->session, response, response, (size_t) packet_get_size(response));
        request->host->send_response(request->session->socket, response, (size_t) packet_get_size(response));
}

static void enter_world(request_t *request)
{
        packet response[SERVER_PACKET_ENTER_WORLD_FULL_SIZE] = {0};
        character_t *character = NULL;

        assert_valid_request(request);

        character = storage_character_active_from_session(&request->storage->character_storage, request->session);

        if (!character) {
                printf("Warning: entering world with missing character.\n");
                printf("Ignoring request\n");
                return;
        }

        session_entered_world(request->session);

        server_packet_enter_world(response, character);
        session_encrypt_packet(request->session, response, response, (size_t) packet_get_size(response));
        request->host->send_response(request->session->socket, response, (size_t) packet_get_size(response));

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
                client_request_quest_list(request->session->socket, request->session, request->host->send_response);
                session_update_state(request->session, ENTERING_WORLD);
                break;
        case CLIENT_PACKET_TYPE_ENTER_WORLD:
                enter_world(request);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_entering_world.\n", type);
                break;
        }
}
