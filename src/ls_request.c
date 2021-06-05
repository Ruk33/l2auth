#include "include/os_socket.h"
#include "include/config.h"
#include "include/util.h"
#include "include/conn.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/server.h"
#include "include/ls_rsa.h"
#include "include/ls_session.h"
#include "include/ls_crypt.h"
#include "include/ls_packet_init.h"
#include "include/ls_packet_gg_auth.h"
#include "include/ls_packet_ok.h"
#include "include/ls_packet_server_list.h"
#include "include/ls_packet_play_ok.h"
#include "include/ls_request.h"

static void handle_gg_auth(ls_session_t *session)
{
        packet_t response[16] = { 0 };

        ls_packet_gg_auth_t gg_auth = { 0 };

        gg_auth.response = PACKET_GG_AUTH_RESPONSE_SKIP;

        ls_packet_gg_auth_pack(response, &gg_auth);
        ls_encrypt(session->blowfish, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_auth_login(ls_session_t *session)
{
        packet_t response[64] = { 0 };

        ls_packet_ok_t ok = { 0 };

        // Todo: Generate these values randomly.
        ok.loginOK1 = 42;
        ok.loginOK2 = 24;

        ls_packet_ok_pack(response, &ok);
        ls_encrypt(session->blowfish, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_request_server_list(ls_session_t *session)
{
        static packet_t response[512] = { 0 };

        static ls_packet_server_list_t server_list = { 0 };

        server_t *servers = 0;

        servers = server_all();

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &server_list, sizeof(server_list));

        for (size_t i = 0, max = server_all_count(); i < max; i += 1) {
                ls_packet_server_list_add(&server_list, &servers[i]);
        }

        ls_packet_server_list_pack(response, &server_list);
        ls_encrypt(session->blowfish, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_login_server(ls_session_t *session)
{
        packet_t response[16] = { 0 };

        ls_packet_play_ok_t play_ok = { 0 };

        log("Player wants to log into game server.");

        play_ok.playOK1 = session->playOK1;
        play_ok.playOK2 = session->playOK2;

        ls_packet_play_ok_pack(response, &play_ok);
        ls_encrypt(session->blowfish, response, response);

        conn_send_packet(session->socket, response);
}

void ls_request_new_conn(os_socket_t *socket)
{
        static packet_t response[256] = { 0 };
        static byte_t modulus[128]    = { 0 };
        static ls_packet_init_t init  = { 0 };

        ls_session_t *session = 0;

        bytes_zero(response, sizeof(response));
        bytes_zero(modulus, sizeof(modulus));
        bytes_zero((byte_t *) &init, sizeof(init));

        session = ls_session_new(socket);

        ls_rsa_modulus(session->rsa, modulus);
        ls_packet_init(&init, modulus);
        ls_packet_init_pack(response, &init);

        conn_send_packet(socket, response);
}

void ls_request(os_socket_t *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        ls_session_t *session = 0;

        u16_t size = 0;

        session = ls_session_find(socket);

        if (!session) {
                log("Session not found. Ignoring request.");
                return;
        }

        bytes_zero(packet, sizeof(packet));
        ls_decrypt(session->blowfish, packet, buf);

        switch (packet_type(packet)) {
        case 0x00: // Auth login
                handle_auth_login(session);
                break;
        case 0x02: // Login server
                handle_login_server(session);
                break;
        case 0x05: // Request server list
                handle_request_server_list(session);
                break;
        case 0x07: // GG Auth
                handle_gg_auth(session);
                break;
        default:
                log("Ignoring unknown packet.");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                ls_request(socket, buf + size, n - size);
        }
}

void ls_request_disconnect(os_socket_t *socket)
{
        PREVENT_UNUSED_WARNING(socket);
}
