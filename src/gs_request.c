#include <assert.h>
#include "include/util.h"
#include "include/list.h"
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

static void handle_protocol_version(struct gs_session *session)
{
        gs_packet_protocol_version_t protocol_version = { 0 };

        packet_t response[16] = { 0 };

        assert(session);

        gs_packet_protocol_version(&protocol_version);
        gs_packet_protocol_version_pack(response, &protocol_version);
        conn_send_packet(session->socket, response);
}

static void
handle_enter_world(struct gs_state *state, struct gs_session *session)
{
        static gs_packet_user_info_t user_info = { 0 };

        static packet_t response[1024] = { 0 };

        struct gs_character *character = 0;

        assert(state);
        assert(session);

        bytes_zero((byte_t *) &user_info, sizeof(user_info));
        bytes_zero(response, sizeof(response));

        character = gs_character_from_session(state, session);

        if (!character) {
                log("entering world without a character? ignoring.");
                return;
        }

        gs_character_spawn(state, character);

        gs_packet_user_info_set_char(&user_info, character);
        gs_packet_user_info_pack(response, &user_info);

        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

// If packet is not NULL, the session will be updated and characters fetched.
// If packet is NULL, only the characters will be fetch (no session update).
static void handle_auth_login(struct gs_session *session, packet_t *packet)
{
        static gs_packet_auth_login_t auth_login = { 0 };

        static struct gs_character characters[10] = { 0 };

        static packet_t response[4096] = { 0 };

        gs_packet_auth_request_t auth_request = { 0 };

        char *username     = 0;
        size_t chars_max   = 0;
        size_t chars_found = 0;

        assert(session);

        bytes_zero((byte_t *) &auth_login, sizeof(auth_login));
        bytes_zero((byte_t *) characters, sizeof(characters));
        bytes_zero(response, sizeof(response));

        if (packet) {
                gs_packet_auth_request_unpack(&auth_request, packet);
                gs_session_update_auth(session, &auth_request);
        }

        username    = session->username;
        chars_max   = arr_size(characters);
        chars_found = storage_get_characters(characters, username, chars_max);

        for (size_t i = 0; i < chars_found; i += 1) {
                characters[i].session = session;
                gs_packet_auth_login_add_character(&auth_login, &characters[i]);
        }

        gs_packet_auth_login_pack(response, &auth_login);
        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

static void handle_new_character(struct gs_session *session)
{
        static gs_packet_new_char_t new_char = { 0 };

        struct gs_character_template *templates = 0;

        packet_t response[1024] = { 0 };

        size_t template_count = 0;

        assert(session);

        bytes_zero((byte_t *) &new_char, sizeof(new_char));
        bytes_zero(response, sizeof(response));

        templates      = gs_character_template_default();
        template_count = gs_character_template_count();

        for (size_t i = 0; i < template_count; i += 1) {
                gs_packet_new_char_add_template(&new_char, &templates[i]);
        }

        gs_packet_new_char_pack(response, &new_char);
        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

static void
handle_create_character(struct gs_session *session, packet_t *packet)
{
        static packet_t response[128] = { 0 };

        gs_packet_create_char_request_t create_char_request = { 0 };

        gs_packet_create_char_t create_char = { 0 };

        struct gs_character character = { 0 };

        assert(session);
        assert(packet);

        bytes_zero(response, sizeof(response));

        gs_packet_create_char_request_unpack(&create_char_request, packet);
        gs_character_from_request(&character, &create_char_request);

        create_char.response =
                storage_create_character(session->username, &character);

        gs_packet_create_char_pack(response, &create_char);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);

        // When a new character is created, the user is taken
        // back to the lobby (where all his/her characters are)
        // this means we have to fetch and re-send the characters.
        // Using a null/empty packet for handle_auth_login
        // will make sure the session isn't updated.
        handle_auth_login(session, 0);
}

static void handle_selected_character(
        struct gs_state *state,
        struct gs_session *session,
        packet_t *packet)
{
        static gs_packet_char_select_t char_select = { 0 };

        static packet_t response[512] = { 0 };

        gs_packet_char_select_request_t char_select_request = { 0 };

        struct gs_character character = { 0 };

        assert(state);
        assert(session);
        assert(packet);

        bytes_zero((byte_t *) &char_select, sizeof(char_select));
        bytes_zero(response, sizeof(response));

        gs_packet_char_select_request_unpack(&char_select_request, packet);

        session->character_index = char_select_request.index;

        storage_get_character(
                &character,
                session->username,
                (size_t) session->character_index);

        character.id      = gs_character_get_free_id(state);
        character.session = session;

        log("selecting character and assigning id %d with session %d",
            character.id,
            session->id);

        gs_character_add(state, &character);

        gs_packet_char_select_set_char(&char_select, &character);
        gs_packet_char_select_set_playok(&char_select, session->playOK1);
        gs_packet_char_select_pack(response, &char_select);

        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

static void handle_quest_list(struct gs_session *session)
{
        gs_packet_quest_list_t quest_list = { 0 };

        packet_t response[16] = { 0 };

        assert(session);

        gs_packet_quest_list_pack(response, &quest_list);
        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

static void handle_auto_ss_bsps(struct gs_session *session)
{
        gs_packet_d0_t d0 = { 0 };

        packet_t response[16] = { 0 };

        assert(session);

        gs_packet_d0_pack(response, &d0);
        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
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
                handle_selected_character(state, session, packet);
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
                log("no character found for session. disconnect?");
                return;
        }

        gs_ai_handle_request(state, character, packet);

        // Todo: refactor. We need to know if the restart was successful.
        // restart packet
        if (packet_type(packet) == 0x46) {
                handle_auth_login(session, 0);
                session->state = CHARACTER_SELECTION;
        }
}

void gs_request_new_conn(struct gs_state *state, struct os_io *socket)
{
        struct gs_session *session = 0;

        if (!state) {
                log("no state passed to gs_request_new_conn? ignoring request.");
                return;
        }

        if (!socket) {
                log("no socket passed to gs_request_new_conn? ignoring request.");
                return;
        }

        session = gs_session_new(state, socket);
        log("new game session with id %d generated.", session->id);
}

void gs_request(
        struct gs_state *state,
        struct os_io *socket,
        byte_t *buf,
        size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        struct gs_session *session = 0;

        size_t safe_packet_clean = 0;

        u16_t size = 0;

        if (!state) {
                log("no state passed to gs_request? ignoring request.");
                return;
        }

        if (!socket) {
                log("no socket passed to gs_request? ignoring request.");
                return;
        }

        if (!buf || !n) {
                log("empty request? ignoring.");
                return;
        }

        session = gs_session_find(state, socket);

        if (!session) {
                log("game server, no session found for request. ignoring.");
                return;
        }

        // Not sure how useful this may be, but it
        // may help reducing the amount of bytes required to
        // be reset.
        safe_packet_clean = min(sizeof(packet), n * 2);
        bytes_zero(packet, safe_packet_clean);

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
                log("session in invalid state. disconnect?");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                gs_request(state, socket, buf + size, n - size);
        }
}

void gs_request_disconnect(struct gs_state *state, struct os_io *socket)
{
        gs_packet_leave_world_t leave_world = { 0 };

        packet_t response[8] = { 0 };

        struct gs_session *session = 0;

        struct gs_character *character = 0;

        if (!state) {
                log("no state passed to gs_request_disconnect? ignoring request.");
                return;
        }

        if (!socket) {
                log("no socket passed to gs_request_disconnect? ignoring request.");
                return;
        }

        session = gs_session_find(state, socket);

        if (!session) {
                log("disconnected client had no session, ignoring.");
                return;
        }

        character = gs_character_from_session(state, session);

        if (character) {
                gs_character_disconnect(state, character);
        }

        log("sending disconnect packet.");

        gs_packet_leave_world_pack(response, &leave_world);
        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);

        gs_session_disconnect(state, session);
}

void gs_request_tick(struct gs_state *state, double delta)
{
        struct gs_character *character = 0;

        if (!state) {
                log("no state for gs_request_tick? ignoring.");
                return;
        }

        list_each(struct gs_character, character, state->list_characters)
        {
                gs_ai_tick(state, character, delta);
        }
}
