#include "include/util.h"
#include "include/socket.h"
#include "include/conn.h"
#include "include/storage.h"
#include "include/log.h"
#include "include/gs_session.h"
#include "include/character.h"
#include "include/character_template.h"
#include "include/packet_protocol_version.h"
#include "include/packet_auth_login.h"
#include "include/packet_auth_request.h"
#include "include/packet_new_char.h"
#include "include/game_server_request.h"

static void handle_protocol_version(gs_session_t *session)
{
        packet_t response[16] = { 0 };

        packet_protocol_version_t protocol_version = { 0 };

        packet_protocol_version(&protocol_version);
        packet_protocol_version_pack(response, &protocol_version);

        conn_send_packet(session->socket, response);
}

static void handle_auth_login(gs_session_t *session, packet_t *packet)
{
        static packet_t response[4096] = { 0 };

        static packet_auth_login_t auth_login = { 0 };

        packet_auth_request_t auth_request = { 0 };

        char *username = 0;

        character_t characters[10]   = { 0 };
        size_t      characters_count = 0;

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &auth_login, sizeof(auth_login));

        packet_auth_request_unpack(&auth_request, packet);
        gs_session_update_auth(session, &auth_request);

        username         = session->username;
        characters_count = storage_get_characters(characters, username, 10);

        for (size_t i = 0; i < characters_count; i += 1) {
                packet_auth_login_add_character(&auth_login, &characters[i]);
        }

        packet_auth_login_pack(response, &auth_login);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_new_character(gs_session_t *session)
{
        static packet_t response[1024] = { 0 };

        static packet_new_char_t new_char = { 0 };

        static character_template_t templates[10] = { 0 };

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &new_char, sizeof(new_char));
        bytes_zero((byte_t *) &templates, sizeof(templates));

        // Human fighter
        templates[0].race   = 0;
        templates[0]._class = 0;
        templates[0].str    = 40;
        templates[0].dex    = 30;
        templates[0].con    = 43;
        templates[0]._int   = 21;
        templates[0].wit    = 11;
        templates[0].men    = 25;

        // Human figher copy
        // Not sure why the clients requires it...
        templates[1].race   = 0;
        templates[1]._class = 0;
        templates[1].str    = 40;
        templates[1].dex    = 30;
        templates[1].con    = 43;
        templates[1]._int   = 21;
        templates[1].wit    = 11;
        templates[1].men    = 25;

        // Human mage
        templates[2].race   = 0;
        templates[2]._class = 10;
        templates[2].str    = 22;
        templates[2].dex    = 21;
        templates[2].con    = 27;
        templates[2]._int   = 41;
        templates[2].wit    = 20;
        templates[2].men    = 39;

        // Elf fighter
        templates[3].race   = 1;
        templates[3]._class = 18;
        templates[3].str    = 36;
        templates[3].dex    = 35;
        templates[3].con    = 36;
        templates[3]._int   = 23;
        templates[3].wit    = 14;
        templates[3].men    = 26;

        // Elf mage
        templates[4].race   = 1;
        templates[4]._class = 25;
        templates[4].str    = 21;
        templates[4].dex    = 24;
        templates[4].con    = 25;
        templates[4]._int   = 37;
        templates[4].wit    = 23;
        templates[4].men    = 40;

        // Dark elf fighter
        templates[5].race   = 2;
        templates[5]._class = 31;
        templates[5].str    = 41;
        templates[5].dex    = 34;
        templates[5].con    = 32;
        templates[5]._int   = 25;
        templates[5].wit    = 12;
        templates[5].men    = 26;

        // Dark elf mage
        templates[6].race   = 2;
        templates[6]._class = 38;
        templates[6].str    = 23;
        templates[6].dex    = 23;
        templates[6].con    = 24;
        templates[6]._int   = 44;
        templates[6].wit    = 19;
        templates[6].men    = 37;

        // Orc fighter
        templates[7].race   = 3;
        templates[7]._class = 44;
        templates[7].str    = 40;
        templates[7].dex    = 26;
        templates[7].con    = 47;
        templates[7]._int   = 18;
        templates[7].wit    = 12;
        templates[7].men    = 27;

        // Orc mage
        templates[8].race   = 3;
        templates[8]._class = 49;
        templates[8].str    = 27;
        templates[8].dex    = 24;
        templates[8].con    = 31;
        templates[8]._int   = 31;
        templates[8].wit    = 15;
        templates[8].men    = 42;

        // Dwarf
        templates[9].race   = 4;
        templates[9]._class = 53;
        templates[9].str    = 39;
        templates[9].dex    = 29;
        templates[9].con    = 45;
        templates[9]._int   = 20;
        templates[9].wit    = 10;
        templates[9].men    = 27;

        for (size_t i = 0; i < 10; i += 1) {
                packet_new_char_add_template(&new_char, &templates[i]);
        }

        packet_new_char_pack(response, &new_char);
        gs_session_encrypt(session, response, response);

        conn_send_packet(session->socket, response);
}

void game_server_request_new_conn(socket_t *socket)
{
        gs_session_new(socket);
}

void game_server_request(socket_t *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        gs_session_t *session = 0;

        u16_t size = 0;

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
        case 0x0e: // New character
                handle_new_character(session);
                break;
        case 0x0b: // Create character
                log("Create character.");
                break;
        case 0x08: // Auth request
                handle_auth_login(session, packet);
                break;
        default:
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                game_server_request(socket, buf + size, n - size);
        }
}

void game_server_request_disconnect(socket_t *socket)
{
        PREVENT_UNUSED_WARNING(socket);
}
