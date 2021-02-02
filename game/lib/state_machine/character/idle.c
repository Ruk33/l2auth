#include <client_request/move.h>
#include <client_request/validate_position.h>
#include <client_request/say.h>
#include <client_request/show_map.h>
#include <client_request/restart.h>
#include <client_request/logout.h>
#include <client_packet/type.h>
#include <storage/character.h>
#include <server_packet/say.h>
#include <client_request/say.h>
#include "idle.h"

/**
 * Send a message to all characters close enough
 * to the speaker.
 */
static void say(request_t *request, character_t *character)
{
        assert_valid_request(request);
        assert(character);

        packet response[SERVER_PACKET_SAY_FULL_SIZE] = {0};
        client_request_say_t say_request = {0};
        struct List *close_characters = NULL;
        struct ListEntry *i_close_character = NULL;
        character_t *close_character = NULL;

        // Hardcoded, not sure if this is the correct maximum
        char raw_message[128] = {0};
        size_t message_len = 0;

        close_characters = list_create(request->host->alloc_memory, request->host->dealloc_memory);

        client_request_say(&say_request, request->packet);
        l2_string_to_char(raw_message, say_request.message, sizeof(raw_message));
        message_len = strlen(raw_message) + 1;

        storage_character_close_to(&request->storage->character_storage, &close_characters, character, CONFIG_SHOUT_RANGE);

        i_close_character = list_get_iterator(close_characters);

        while (i_close_character) {
                close_character = list_get_value(i_close_character);

                server_packet_say(response, character, say_request.type, raw_message, message_len);
                session_encrypt_packet(close_character->session, response, response, (size_t) packet_get_size(response));
                request->host->send_response(close_character->session->socket, response, (size_t) packet_get_size(response));

                memset(response, 0, sizeof(response));

                i_close_character = list_get_next(i_close_character);
        }

        list_free(close_characters);
}

void state_machine_character_idle(request_t *request, character_t *character)
{
        assert_valid_request(request);
        assert(character);

        packet_type type = 0;
        int socket = 0;

        type = packet_get_type(request->packet);
        socket = request->session->socket;

        switch (type) {
        case CLIENT_PACKET_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                client_request_move(socket, request->packet, request->session, character, request->host->send_response);
                break;
        case CLIENT_PACKET_TYPE_VALIDATE_POS:
                client_request_validate_position(socket, request->packet, request->session, character, request->host->send_response);
                break;
        case CLIENT_PACKET_TYPE_ACTION:
                printf("Action request -> TODO\n");
                break;
        case CLIENT_PACKET_TYPE_SAY:
                say(request, character);
                break;
        case CLIENT_PACKET_TYPE_SHOW_MAP:
                client_request_show_map(socket, request->session, request->host->send_response);
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
                printf("Packet %02X can't be handled by state_machine_character_idle.\n", type);
                break;
        }
}
