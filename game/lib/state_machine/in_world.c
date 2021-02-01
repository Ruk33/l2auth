#include "../client_request/move.h"
#include "../client_request/validate_position.h"
#include "../client_request/say.h"
#include "../client_request/show_map.h"
#include "../client_request/restart.h"
#include "../client_request/logout.h"
#include "../client_packet/type.h"
#include "../storage/character.h"
#include "in_world.h"

void state_machine_in_world(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;
        int socket = 0;
        character_t *character = NULL;

        type = packet_get_type(request->packet);
        socket = request->session->socket;
        character = storage_character_active_from_session(
                &request->storage->character_storage,
                request->session
        );

        if (!character) {
                printf("In world but no character? Ignoring request.\n");
                return;
        }

        switch (type) {
        case CLIENT_PACKET_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                client_request_move(socket, request->packet, request->session, character, request->host->send_response);
                session_update_state(request->session, IN_WORLD);
                break;
        case CLIENT_PACKET_TYPE_VALIDATE_POS:
                client_request_validate_position(socket, request->packet, request->session, character, request->host->send_response);
                session_update_state(request->session, IN_WORLD);
                break;
        case CLIENT_PACKET_TYPE_ACTION:
                printf("Action request -> TODO\n");
                session_update_state(request->session, IN_WORLD);
                break;
        case CLIENT_PACKET_TYPE_SAY:
                client_request_say(socket, request->packet, request->session, character, request->host->send_response);
                session_update_state(request->session, IN_WORLD);
                break;
        case CLIENT_PACKET_TYPE_SHOW_MAP:
                client_request_show_map(socket, request->session, request->host->send_response);
                session_update_state(request->session, IN_WORLD);
                break;
        case CLIENT_PACKET_TYPE_RESTART:
                client_request_restart(socket, request->session, &request->storage->character_storage, request->host->send_response);
                session_update_state(request->session, CHARACTER_SELECTION);
                break;
        case CLIENT_PACKET_TYPE_LOGOUT:
                client_request_logout(socket, request->session, request->host->send_response);
                session_update_state(request->session, PROTOCOL_VERSION);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_in_world.\n", type);
                break;
        }
}
