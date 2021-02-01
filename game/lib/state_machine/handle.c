#include "protocol_version.h"
#include "auth_request.h"
#include "character_selection.h"
#include "creating_character.h"
#include "entering_world.h"
#include "in_world.h"
#include "handle.h"

void state_machine_handle(int client, byte *request_packet, size_t request_size, host_t *host, storage_server_t *server_storage)
{
        assert(request_packet);
        assert(host);
        assert(server_storage);

        request_t request = {0};
        session_t *session = NULL;

        session = storage_session_get(&server_storage->session_storage, client);

        if (!session)
        {
                printf("Warning: session for client %d not found.\n", client);
                printf("Ignoring request.\n");
                return;
        }

        session_decrypt_packet(session, request_packet, request_packet, packet_get_size(request_packet));
        session_encrypt_connection(session);

        request.host = host;
        request.session = session;
        request.storage = server_storage;
        request.packet = request_packet;
        request.size = request_size;

        switch (session->state) {
        case PROTOCOL_VERSION:
                state_machine_protocol_version(&request);
                break;
        case AUTH_REQUEST:
                state_machine_auth_request(&request);
                break;
        case CHARACTER_SELECTION:
                state_machine_character_selection(&request);
                break;
        case CREATING_CHARACTER:
                state_machine_creating_character(&request);
                break;
        case ENTERING_WORLD:
                state_machine_entering_world(&request);
                break;
        case IN_WORLD:
                state_machine_in_world(&request);
                break;
        default:
                printf("Invalid state from client. Ignoring request.\n");
                break;
        }

        fflush(stdout);
}
