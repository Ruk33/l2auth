#include <assert.h>
#include "include/util.h"
#include "include/os_socket.h"
#include "include/conn.h"
#include "include/storage.h"
#include "include/log.h"
#include "include/l2_string.h"
#include "include/packet.h"
#include "include/gs_session.h"
#include "include/gs_character.h"
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
#include "include/gs_request.h"

static void handle_protocol_version(gs_session_t *session)
{
        packet_t response[16] = { 0 };

        gs_packet_protocol_version_t protocol_version = { 0 };

        assert(session);

        gs_packet_protocol_version(&protocol_version);
        gs_packet_protocol_version_pack(response, &protocol_version);

        conn_send_packet(session->socket, response);
}

static void handle_enter_world(gs_session_t *session)
{
        static packet_t response[1024] = { 0 };

        static gs_packet_user_info_t user_info = { 0 };

        gs_character_t character = { 0 };

        assert(session);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &user_info, sizeof(user_info));

        // Todo check which character the client selected.
        if (!storage_get_characters(&character, session->username, 1)) {
                log("Character not found.");
                return;
        }

        gs_packet_user_info_set_char(&user_info, &character);
        gs_packet_user_info_pack(response, &user_info);

        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

// If packet is not NULL, the session will be updated and characters fetched.
// If packet is NULL, only the characters will be fetch (no session update).
static void handle_auth_login(gs_session_t *session, packet_t *packet)
{
        static packet_t response[4096] = { 0 };

        static gs_packet_auth_login_t auth_login = { 0 };

        static gs_character_t characters[10] = { 0 };

        gs_packet_auth_request_t auth_request = { 0 };

        char *username = 0;

        size_t characters_count = 0;

        assert(session);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &auth_login, sizeof(auth_login));
        bytes_zero((byte_t *) characters, sizeof(characters));

        if (packet) {
                gs_packet_auth_request_unpack(&auth_request, packet);
                gs_session_update_auth(session, &auth_request);
        }

        username         = session->username;
        characters_count = storage_get_characters(characters, username, 10);

        for (size_t i = 0; i < characters_count; i += 1) {
                gs_packet_auth_login_add_character(&auth_login, &characters[i]);
        }

        gs_packet_auth_login_pack(response, &auth_login);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_new_character(gs_session_t *session)
{
        static packet_t response[1024] = { 0 };

        static gs_packet_new_char_t new_char = { 0 };

        gs_character_template_t *templates = 0;

        size_t template_count = 0;

        assert(session);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &new_char, sizeof(new_char));

        templates      = gs_character_template_default();
        template_count = gs_character_template_count();

        for (size_t i = 0; i < template_count; i += 1) {
                gs_packet_new_char_add_template(&new_char, &templates[i]);
        }

        gs_packet_new_char_pack(response, &new_char);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_create_character(gs_session_t *session, packet_t *packet)
{
        packet_t response[8] = { 0 };

        gs_packet_create_char_request_t create_char_request = { 0 };

        gs_packet_create_char_t create_char = { 0 };

        gs_character_t character = { 0 };

        assert(session);
        assert(packet);

        gs_packet_create_char_request_unpack(&create_char_request, packet);
        gs_character_from_request(&character, &create_char_request);

        create_char.response =
                storage_create_character(session->username, &character);

        gs_packet_create_char_pack(response, &create_char);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);

        // Refetch characters.
        handle_auth_login(session, 0);
}

static void handle_selected_character(gs_session_t *session, packet_t *packet)
{
        static packet_t response[512] = { 0 };

        static gs_packet_char_select_t char_select = { 0 };

        gs_packet_char_select_request_t char_select_request = { 0 };

        gs_character_t character = { 0 };

        assert(session);
        assert(packet);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &char_select, sizeof(char_select));

        gs_packet_char_select_request_unpack(&char_select_request, packet);

        if (!storage_get_characters(&character, session->username, 1)) {
                log("Character not found");
                return;
        }

        gs_packet_char_select_set_char(&char_select, &character);
        gs_packet_char_select_set_playok(&char_select, session->playOK1);
        gs_packet_char_select_pack(response, &char_select);

        gs_session_encrypt(session, response, response);
        conn_send_packet(session->socket, response);
}

static void handle_quest_list(gs_session_t *session)
{
        packet_t response[16] = { 0 };

        gs_packet_quest_list_t quest_list = { 0 };

        assert(session);

        gs_packet_quest_list_pack(response, &quest_list);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_auto_ss_bsps(gs_session_t *session)
{
        packet_t response[16] = { 0 };

        gs_packet_d0_t d0 = { 0 };

        assert(session);

        gs_packet_d0_pack(response, &d0);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

void gs_request_new_conn(os_socket_t *socket)
{
        assert(socket);
        gs_session_new(socket);
}

void gs_request(os_socket_t *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        gs_session_t *session = 0;

        u16_t size = 0;

        assert(socket);

        session = gs_session_find(socket);

        if (!session) {
                log("Game server, no session found for request. Ignoring.");
                return;
        }

        bytes_zero(packet, sizeof(packet));

        gs_session_decrypt(session, packet, buf);
        gs_session_encrypt_conn(session);

        switch (packet_type(packet)) {
        case 0x00: // Protocol version
                handle_protocol_version(session);
                break;
        case 0x01: // Move backwards.
                log("TODO: Move backwards");
                break;
        case 0x03: // Enter world.
                handle_enter_world(session);
                break;
        case 0x04: // Action.
                log("TODO: Action");
                break;
        case 0x08: // Auth request
                handle_auth_login(session, packet);
                break;
        case 0x09: // Logout.
                log("TODO: Logout");
                break;
        case 0x0b: // Create character
                handle_create_character(session, packet);
                break;
        case 0x0d: // Selected char.
                handle_selected_character(session, packet);
                break;
        case 0x0e: // New character
                handle_new_character(session);
                break;
        case 0x38: // Say.
                log("TODO: Say");
                break;
        case 0x46: // Restart.
                log("TODO: Restart");
                break;
        case 0x48: // Validate position.
                log("TODO: Validate position");
                break;
        case 0x63: // Quest list.
                handle_quest_list(session);
                break;
        case 0xcd: // Show map.
                log("TODO: Show map");
                break;
        case 0xd0: // Auto ss bsps.
                handle_auto_ss_bsps(session);
                break;
        default:
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                gs_request(socket, buf + size, n - size);
        }
}

void gs_request_disconnect(os_socket_t *socket)
{
        PREVENT_UNUSED_WARNING(socket);
}