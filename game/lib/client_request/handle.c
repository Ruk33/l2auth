#include "../headers.h"
#include "../decrypt_packet.h"
#include "../client_packet/type.h"
#include "../storage/server.h"
#include "../storage/session.h"
#include "../session.h"
#include "../character.h"
#include "protocol_version.h"
#include "auth_request.h"
#include "new_char.h"
#include "create_char.h"
#include "select_character.h"
#include "d0.h"
#include "quest_list.h"
#include "enter_world.h"
#include "move.h"
#include "validate_position.h"
#include "restart.h"
#include "show_map.h"
#include "logout.h"
#include "say.h"
#include "handle.h"

void client_request_handle(
        int client,
        byte *request,
        size_t request_size,
        struct Host *host,
        storage_server_t *server_storage
)
{
        assert(client > 0);
        assert(request);
        assert(request_size);
        assert(host);
        assert(server_storage);

        session_t *session = storage_session_get(
                &server_storage->session_storage,
                client
        );

        character_t *character = NULL;

        if (!session)
        {
                printf("Warning: session for client %d not found.\n", client);
                printf("Ignoring request.\n");
                return;
        }

        character = storage_character_active_from_session(
                &server_storage->character_storage,
                session
        );

        if (session->in_world && !character)
        {
                printf("Warning: in game session has no active character.\n");
                printf("Client %d has no active character.\n", client);
                printf("Ignoring request.\n");
                return;
        }

        session_decrypt_packet(session, request, request, packet_get_size(request));
        session_encrypt_connection(session);

        printf("Packet type %02X.\n", packet_get_type(request));

        switch (packet_get_type(request))
        {
        case CLIENT_PACKET_TYPE_PROTOCOL_VERSION:
                client_request_protocol_version(
                        client,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                client_request_move(
                        client,
                        request,
                        session,
                        character,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_ACTION:
                printf("Packet action, todo.\n");
                break;
        case CLIENT_PACKET_TYPE_AUTH_REQUEST:
                client_request_auth_request(
                        client,
                        request,
                        &server_storage->character_storage,
                        session,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_NEW_CHAR:
                client_request_new_char(
                        client,
                        session,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_CREATE_CHAR:
                client_request_create_char(
                        client,
                        request,
                        session,
                        &server_storage->character_storage,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_SELECTED_CHAR:
                client_request_select_character(
                        client,
                        request,
                        session,
                        &server_storage->character_storage,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_REQUEST_AUTO_SS_BSPS:
                client_request_d0(
                        client,
                        session,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_REQUEST_QUEST_LIST:
                client_request_quest_list(
                        client,
                        session,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_ENTER_WORLD:
                client_request_enter_world(
                        client,
                        session,
                        &server_storage->character_storage,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_RESTART:
                client_request_restart(
                        client,
                        session,
                        &server_storage->character_storage,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_VALIDATE_POS:
                client_request_validate_position(
                        client,
                        request,
                        session,
                        character,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_SAY:
                client_request_say(
                        client,
                        request,
                        session,
                        character,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_LOGOUT:
                client_request_logout(
                        client,
                        session,
                        host->send_response
                );
                break;
        case CLIENT_PACKET_TYPE_SHOW_MAP:
                client_request_show_map(
                        client,
                        session,
                        host->send_response
                );
                break;
        default:
                printf("Unknown packet. Ignoring...\n");
                break;
        }
}
