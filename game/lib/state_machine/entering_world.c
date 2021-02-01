#include "../client_request/d0.h"
#include "../client_request/quest_list.h"
#include "../client_request/enter_world.h"
#include "../client_packet/type.h"
#include "entering_world.h"

void state_machine_entering_world(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_REQUEST_AUTO_SS_BSPS:
                client_request_d0(request->session->socket, request->session, request->host->send_response);
                session_update_state(request->session, ENTERING_WORLD);
                break;
        case CLIENT_PACKET_TYPE_REQUEST_QUEST_LIST:
                client_request_quest_list(request->session->socket, request->session, request->host->send_response);
                session_update_state(request->session, ENTERING_WORLD);
                break;
        case CLIENT_PACKET_TYPE_ENTER_WORLD:
                client_request_enter_world(request->session->socket, request->session, &request->storage->character_storage, request->host->send_response);
                session_update_state(request->session, IN_WORLD);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_entering_world.\n", type);
                break;
        }
}
