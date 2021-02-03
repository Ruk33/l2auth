#include <request.h>
#include <client_request/create_char.h>
#include <client_packet/type.h>
#include "character_selection.h"
#include "creating_character.h"

void state_machine_creating_character(request_t *request)
{
        assert_valid_request(request);

        packet_type type = 0;

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_CREATE_CHAR:
                client_request_create_char(request->session->socket, request->packet, request->session, &request->storage->character_storage, request->host->send_response);
                session_update_state(request->session, CHARACTER_SELECTION);
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
