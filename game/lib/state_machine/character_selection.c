#include <client_request/new_char.h>
#include <client_request/select_character.h>
#include <client_packet/type.h>
#include "character_selection.h"

void state_machine_character_selection(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_NEW_CHAR:
                client_request_new_char(request->session->socket, request->session, request->host->send_response);
                session_update_state(request->session, CREATING_CHARACTER);
                break;
        case CLIENT_PACKET_TYPE_SELECTED_CHAR:
                client_request_select_character(request->session->socket, request->packet, request->session, &request->storage->character_storage, request->host->send_response);
                session_update_state(request->session, ENTERING_WORLD);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_character_selection.\n", type);
                break;
        }
}
