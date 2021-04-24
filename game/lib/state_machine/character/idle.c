#include <request.h>
#include <util/session_crypt.h>
#include <client_request/move.h>
#include <client_request/validate_position.h>
#include <client_request/say.h>
#include <client_packet/type.h>
#include <db/character.h>
#include <server_packet/say.h>
#include <server_packet/logout.h>
#include <server_packet/restart.h>
#include <server_packet/move.h>
#include <server_packet/validate_position.h>
#include <server_packet/show_map.h>
#include <server_packet/npc_info.h>
#include <client_request/say.h>
#include "../auth_request.h"
#include "idle.h"

/**
 * The player clicks on logout.
 * Do not confuse with losing the connection.
 */
static void logout(request_t *request, character_t *character)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_logout_t)] = { 0 };

        assert_valid_request(request);
        assert(character);

        server_packet_logout(response);
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

        /*
         * Maybe here would be a good moment
         * to persist the character into the database.
         */
        character_update_state(character, SPAWN);

        session_update_state(request->session, PROTOCOL_VERSION);
}

/**
 * The user clicks on restart.
 * This is where we take the user back
 * to the character selection screen.
 */
static void restart(request_t *request, character_t *character)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_restart_t)] = { 0 };
        packet request_characters_packet[8]                             = { 0 };
        request_t request_characters                                    = { 0 };

        assert_valid_request(request);
        assert(character);

        server_packet_restart(response);
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

        character_update_state(character, SPAWN);

        /*
         * Create a dummy request to refetch all
         * characters from a session.
         */
        packet_build(
                request_characters_packet,
                (unsigned char) CLIENT_PACKET_TYPE_AUTH_REQUEST,
                NULL,
                0);

        memcpy(&request_characters, request, sizeof(request_characters));
        request_characters.packet = request_characters_packet;

        state_machine_auth_request(&request_characters, 0);
}

/**
 * The player clicks to move to a particular
 * position.
 */
static void move(request_t *request, character_t *character)
{
        client_request_move_t parsed_request                         = { 0 };
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_move_t)] = { 0 };

        size_t       max_close_characters   = 0;
        size_t       close_characters_count = 0;
        character_t *close_characters       = NULL;
        character_t *close_character        = NULL;
        position_t   character_position     = { 0 };

        assert_valid_request(request);
        assert(character);

        client_request_move(&parsed_request, request->packet);

        max_close_characters = 10;
        close_characters     = request->host->alloc_memory(
                sizeof(*close_characters) * max_close_characters);
        character_position.x   = character->x;
        character_position.y   = character->y;
        character_position.z   = character->z;
        close_characters_count = db_character_in_radius(
                request->storage,
                close_characters,
                max_close_characters,
                &character_position,
                1200);

        for (size_t i = 0; i < close_characters_count; i++) {
                close_character = &close_characters[i];

                server_packet_move(
                        response, character, &parsed_request.position);
                util_session_encrypt_packet(
                        request->storage,
                        close_character->session->socket,
                        response,
                        response,
                        (size_t) packet_get_size(response));
                request->host->send_response(
                        close_character->session->socket,
                        response,
                        (size_t) packet_get_size(response));
        }

        character_move(character, &parsed_request.position);
        request->host->dealloc_memory(close_characters);
}

/**
 * The client performs this request every 1 second (double check)
 * to make sure the client and server are in sync with
 * a character's position.
 */
static void validate_position(request_t *request, character_t *character)
{
        packet response[PACKET_SAFE_FULL_SIZE(
                server_packet_validate_position_t)] = { 0 };

        client_request_validate_position_t parsed_request     = { 0 };
        position_t                         validated_position = { 0 };

        assert_valid_request(request);
        assert(character);

        client_request_validate_position(&parsed_request, request->packet);

        character_validate_position(
                character,
                &validated_position,
                &parsed_request.position,
                parsed_request.heading);

        server_packet_validate_position(
                response,
                character,
                &validated_position,
                parsed_request.heading);
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

/*
 * Test code, remove!
 */
static void spawn_random_orc(request_t *request, position_t *position)
{
        size_t  packet_safe_size = 0;
        packet *npc_info_packet  = NULL;

        character_t orc = { 0 };

        assert_valid_request(request);
        assert(position);

        packet_safe_size =
                PACKET_SAFE_FULL_SIZE(sizeof(server_packet_npc_info_t));
        npc_info_packet = request->host->alloc_memory(packet_safe_size);

        orc.id = rand();
        orc.x  = position->x;
        orc.y  = position->y;
        orc.z  = position->z;

        strcat(orc.name, "Orc");
        strcat(orc.title, "Archer");

        server_packet_npc_info(npc_info_packet, &orc);
        util_session_encrypt_packet(
                request->storage,
                request->session->socket,
                npc_info_packet,
                npc_info_packet,
                (size_t) packet_get_size(npc_info_packet));
        request->host->send_response(
                request->session->socket,
                npc_info_packet,
                (size_t) packet_get_size(npc_info_packet));

        request->host->dealloc_memory(npc_info_packet);
}

/**
 * Send a message to all characters close enough
 * to the speaker.
 */
static void say(request_t *request, character_t *character)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_say_t)] = { 0 };
        client_request_say_t say_request                            = { 0 };

        size_t       close_characters_count = 0;
        size_t       max_close_characters   = 0;
        character_t *close_characters       = NULL;
        character_t *close_character        = NULL;

        position_t orc_position       = { 0 };
        position_t character_position = { 0 };

        // Hardcoded, not sure if this is the correct maximum
        char   raw_message[128] = { 0 };
        size_t message_len      = 0;

        assert_valid_request(request);
        assert(character);

        max_close_characters = 10;
        close_characters     = request->host->alloc_memory(
                sizeof(*close_characters) * max_close_characters);
        character_position.x   = character->x;
        character_position.y   = character->y;
        character_position.z   = character->z;
        close_characters_count = db_character_in_radius(
                request->storage,
                close_characters,
                max_close_characters,
                &character_position,
                CONFIG_SHOUT_RANGE);

        client_request_say(&say_request, request->packet);
        l2_string_to_char(
                raw_message, say_request.message, sizeof(raw_message));
        message_len = strlen(raw_message) + 1;

        for (size_t i = 0; i < close_characters_count; i++) {
                close_character = &close_characters[i];

                server_packet_say(
                        response,
                        character,
                        say_request.type,
                        raw_message,
                        message_len);
                util_session_encrypt_packet(
                        request->storage,
                        close_character->session->socket,
                        response,
                        response,
                        (size_t) packet_get_size(response));
                request->host->send_response(
                        close_character->session->socket,
                        response,
                        (size_t) packet_get_size(response));
        }

        /*
         * Test code, remove!
         */
        if (strcmp("orc", raw_message) == 0) {
                orc_position.x = character->x;
                orc_position.y = character->y;
                orc_position.z = character->z;
                spawn_random_orc(request, &orc_position);
        }

        request->host->dealloc_memory(close_characters);
}

/**
 * In game, the user clicks on display map.
 */
static void show_map(request_t *request, character_t *character)
{
        packet response[PACKET_SAFE_FULL_SIZE(server_packet_show_map_t)] = { 0 };

        assert_valid_request(request);
        assert(character);

        // Map id hardcoded just for the time being.
        server_packet_show_map(response, 1665);
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

void state_machine_character_idle(request_t *request, character_t *character)
{
        packet_type type = 0;

        assert_valid_request(request);
        assert(character);

        type = packet_get_type(request->packet);

        switch (type) {
        case CLIENT_PACKET_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                move(request, character);
                break;
        case CLIENT_PACKET_TYPE_VALIDATE_POS:
                validate_position(request, character);
                break;
        case CLIENT_PACKET_TYPE_ACTION:
                printf("Action request -> TODO\n");
                break;
        case CLIENT_PACKET_TYPE_SAY:
                say(request, character);
                break;
        case CLIENT_PACKET_TYPE_SHOW_MAP:
                show_map(request, character);
                break;
        case CLIENT_PACKET_TYPE_RESTART:
                restart(request, character);
                break;
        case CLIENT_PACKET_TYPE_LOGOUT:
                logout(request, character);
                break;
        default:
                printf("Packet %02X can't be handled by state_machine_character_idle.\n",
                       type);
                break;
        }
}
