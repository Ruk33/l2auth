#include <assert.h>
#include "include/util.h"
#include "include/os_io.h"
#include "include/conn.h"
#include "include/storage.h"
#include "include/log.h"
#include "include/l2_string.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_character.h"
#include "include/gs_session.h"
#include "include/gs_ai.h"
#include "include/gs_character_template.h"
#include "include/gs_packet_protocol_version.h"
#include "include/gs_packet_auth_login.h"
#include "include/gs_packet_auth_request.h"
#include "include/gs_packet_new_char.h"
#include "include/gs_packet_create_char_request.h"
#include "include/gs_packet_create_char.h"
#include "include/gs_packet_char_select_request.h"
#include "include/gs_packet_char_select.h"
#include "include/gs_packet_d0.h"
#include "include/gs_packet_quest_list.h"
#include "include/gs_packet_user_info.h"
#include "include/gs_packet_leave_world.h"
#include "include/gs_request.h"

// Todo: do we really need this?
static packet_t gs_response[65536] = { 0 };

static void handle_protocol_version(struct gs_session *session)
{
        gs_packet_protocol_version_t protocol_version = { 0 };

        assert(session);

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_protocol_version(&protocol_version);
        gs_packet_protocol_version_pack(gs_response, &protocol_version);

        conn_send_packet(session->socket, gs_response);
}

static void
handle_enter_world(struct gs_state *state, struct gs_session *session)
{
        static gs_packet_user_info_t user_info = { 0 };

        struct gs_character character = { 0 };

        assert(state);
        assert(session);

        bytes_zero((byte_t *) &user_info, sizeof(user_info));

        // Todo check which character the client selected.
        if (!storage_get_characters(&character, session->username, 1)) {
                log("Character not found.");
                return;
        }

        character.session = session;
        character.id      = session->id;

        gs_character_spawn(state, &character);

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_user_info_set_char(&user_info, &character);
        gs_packet_user_info_pack(gs_response, &user_info);

        gs_session_encrypt(session, gs_response, gs_response);
        conn_send_packet(session->socket, gs_response);
}

// If packet is not NULL, the session will be updated and characters fetched.
// If packet is NULL, only the characters will be fetch (no session update).
static void handle_auth_login(struct gs_session *session, packet_t *packet)
{
        static gs_packet_auth_login_t auth_login = { 0 };

        static struct gs_character characters[10] = { 0 };

        gs_packet_auth_request_t auth_request = { 0 };

        char *username = 0;

        size_t characters_count = 0;

        assert(session);

        bytes_zero((byte_t *) &auth_login, sizeof(auth_login));
        bytes_zero((byte_t *) characters, sizeof(characters));

        if (packet) {
                gs_packet_auth_request_unpack(&auth_request, packet);
                gs_session_update_auth(session, &auth_request);
        }

        username         = session->username;
        characters_count = storage_get_characters(characters, username, 10);

        for (size_t i = 0; i < characters_count; i += 1) {
                characters[i].session = session;
                gs_packet_auth_login_add_character(&auth_login, &characters[i]);
        }

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_auth_login_pack(gs_response, &auth_login);
        gs_session_encrypt(session, gs_response, gs_response);

        conn_send_packet(session->socket, gs_response);
}

static void handle_new_character(struct gs_session *session)
{
        static gs_packet_new_char_t new_char = { 0 };

        struct gs_character_template *templates = 0;

        size_t template_count = 0;

        assert(session);

        bytes_zero((byte_t *) &new_char, sizeof(new_char));

        templates      = gs_character_template_default();
        template_count = gs_character_template_count();

        for (size_t i = 0; i < template_count; i += 1) {
                gs_packet_new_char_add_template(&new_char, &templates[i]);
        }

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_new_char_pack(gs_response, &new_char);
        gs_session_encrypt(session, gs_response, gs_response);

        conn_send_packet(session->socket, gs_response);
}

static void
handle_create_character(struct gs_session *session, packet_t *packet)
{
        gs_packet_create_char_request_t create_char_request = { 0 };

        gs_packet_create_char_t create_char = { 0 };

        struct gs_character character = { 0 };

        assert(session);
        assert(packet);

        gs_packet_create_char_request_unpack(&create_char_request, packet);
        gs_character_from_request(&character, &create_char_request);

        create_char.response =
                storage_create_character(session->username, &character);

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_create_char_pack(gs_response, &create_char);
        gs_session_encrypt(session, gs_response, gs_response);

        conn_send_packet(session->socket, gs_response);

        // Refetch characters.
        handle_auth_login(session, 0);
}

static void
handle_selected_character(struct gs_session *session, packet_t *packet)
{
        static gs_packet_char_select_t char_select = { 0 };

        gs_packet_char_select_request_t char_select_request = { 0 };

        struct gs_character character = { 0 };

        assert(session);
        assert(packet);

        bytes_zero((byte_t *) &char_select, sizeof(char_select));

        gs_packet_char_select_request_unpack(&char_select_request, packet);

        if (!storage_get_characters(&character, session->username, 1)) {
                log("character not found");
                return;
        }

        bytes_zero(gs_response, sizeof(gs_response));

        character.session = session;

        gs_packet_char_select_set_char(&char_select, &character);
        gs_packet_char_select_set_playok(&char_select, session->playOK1);
        gs_packet_char_select_pack(gs_response, &char_select);

        gs_session_encrypt(session, gs_response, gs_response);
        conn_send_packet(session->socket, gs_response);
}

static void handle_quest_list(struct gs_session *session)
{
        gs_packet_quest_list_t quest_list = { 0 };

        assert(session);

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_quest_list_pack(gs_response, &quest_list);
        gs_session_encrypt(session, gs_response, gs_response);

        conn_send_packet(session->socket, gs_response);
}

static void handle_auto_ss_bsps(struct gs_session *session)
{
        gs_packet_d0_t d0 = { 0 };

        assert(session);

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_d0_pack(gs_response, &d0);
        gs_session_encrypt(session, gs_response, gs_response);

        conn_send_packet(session->socket, gs_response);
}

static void protocol_version_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        assert(state);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x00: // Protocol version
                handle_protocol_version(session);
                session->state = AUTH_REQUEST;
                break;
        default:
                log("can't handle packet from protocol version state.");
                break;
        }
}

static void auth_request_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        assert(state);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x08: // Auth request
                handle_auth_login(session, packet);
                session->state = CHARACTER_SELECTION;
                break;
        default:
                log("can't handle packet from auth request state.");
                break;
        }
}

static void character_selection_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        assert(state);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x0d: // Selected char.
                handle_selected_character(session, packet);
                session->state = ENTERING_WORLD;
                break;
        case 0x0e: // New character
                handle_new_character(session);
                session->state = CREATING_CHARACTER;
                break;
        default:
                log("can't handle packet from character selection state.");
                break;
        }
}

static void creating_character_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        assert(state);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x0b: // Create character
                handle_create_character(session, packet);
                session->state = CHARACTER_SELECTION;
                break;
        default:
                /*
                 * When creating a new character, the user can go
                 * back to the character selection screen and no
                 * packet will be sent. This is why we also need to
                 * accept states from character selection.
                 */
                log("packet delegated from creating character to character selection.");
                character_selection_state(state, session, packet);
                break;
        }
}

static void entering_world_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        assert(state);
        assert(session);
        assert(packet);

        switch (packet_type(packet)) {
        case 0x03: // Enter world.
                handle_enter_world(state, session);
                session->state = IN_WORLD;
                break;
        case 0x63: // Quest list.
                handle_quest_list(session);
                break;
        case 0xd0: // Auto ss bsps.
                handle_auto_ss_bsps(session);
                break;
        default:
                log("can't handle packet from entering world state.");
                break;
        }
}

static void in_world_state(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        struct gs_character *character = 0;

        assert(state);
        assert(session);
        assert(packet);

        character = gs_character_from_session(state, session);

        if (!character) {
                log("no character found for session. Disconnect?");
                return;
        }

        gs_ai_handle_request(state, character, packet);
}

void gs_request_new_conn(struct gs_state *state, os_io_t *socket)
{
        struct gs_session *session = 0;

        assert(state);
        assert(socket);

        session = gs_session_new(state, socket);
        log("new game session with id %d generated.", session->id);
}

void gs_request(struct gs_state *state, os_io_t *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        struct gs_session *session = 0;

        u16_t size = 0;

        assert(state);
        assert(socket);

        session = gs_session_find(state, socket);

        if (!session) {
                log("game server, no session found for request. Ignoring.");
                return;
        }

        bytes_zero(packet, sizeof(packet));

        gs_session_decrypt(session, packet, buf);
        gs_session_encrypt_conn(session);

        switch (session->state) {
        case PROTOCOL_VERSION:
                protocol_version_state(state, session, packet);
                break;
        case AUTH_REQUEST:
                auth_request_state(state, session, packet);
                break;
        case CHARACTER_SELECTION:
                character_selection_state(state, session, packet);
                break;
        case CREATING_CHARACTER:
                creating_character_state(state, session, packet);
                break;
        case ENTERING_WORLD:
                entering_world_state(state, session, packet);
                break;
        case IN_WORLD:
                in_world_state(state, session, packet);
                break;
        default:
                log("session in invalid state. Disconnect?");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                gs_request(state, socket, buf + size, n - size);
        }
}

void gs_request_disconnect(struct gs_state *state, os_io_t *socket)
{
        gs_packet_leave_world_t leave_world = { 0 };

        struct gs_session *session = 0;

        assert(state);

        session = gs_session_find(state, socket);

        if (!session) {
                log("disconnected client had no session, ignoring.");
                return;
        }

        log("sending disconnect packet.");

        bytes_zero(gs_response, sizeof(gs_response));
        gs_packet_leave_world_pack(gs_response, &leave_world);
        gs_session_encrypt(session, gs_response, gs_response);
        conn_send_packet(session->socket, gs_response);
}

void gs_request_tick(struct gs_state *state, double delta)
{
        struct gs_character *characters = 0;

        assert(state);

        characters = state->characters;

        for (size_t i = 0, max = state->character_count; i < max; i += 1) {
                gs_ai_tick(state, &characters[i], delta);
        }
}
