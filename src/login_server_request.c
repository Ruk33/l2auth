#include "include/config.h"
#include "include/util.h"
#include "include/socket.h"
#include "include/rsa.h"
#include "include/packet.h"
#include "include/session.h"
#include "include/server.h"
#include "include/packet_init.h"
#include "include/packet_gg_auth.h"
#include "include/packet_ok.h"
#include "include/packet_server_list.h"
#include "include/packet_play_ok.h"
#include "include/login_server.h"
#include "include/login_server_crypt.h"
#include "include/login_server_request.h"

static void handle_gg_auth(session_t *session)
{
        packet_t response[16] = { 0 };

        packet_gg_auth_t gg_auth = { 0 };

        gg_auth.response = PACKET_GG_AUTH_RESPONSE_SKIP;

        packet_gg_auth_pack(response, &gg_auth);
        login_server_encrypt(session->blowfish, response, response);

        login_server_send_packet(session->socket, response);
}

static void handle_auth_login(session_t *session)
{
        packet_t response[64] = { 0 };

        packet_ok_t ok = { 0 };

        // Todo: Generate these values randomly.
        ok.loginOK1 = 42;
        ok.loginOK2 = 24;

        packet_ok_pack(response, &ok);
        login_server_encrypt(session->blowfish, response, response);

        login_server_send_packet(session->socket, response);
}

static void handle_request_server_list(session_t *session)
{
        static packet_t response[512] = { 0 };

        static packet_server_list_t server_list = { 0 };

        server_t *servers = 0;

        servers = server_all();

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &server_list, sizeof(server_list));

        for (size_t i = 0, max = server_all_count(); i < max; i += 1) {
                packet_server_list_add(&server_list, &servers[i]);
        }

        packet_server_list_pack(response, &server_list);
        login_server_encrypt(session->blowfish, response, response);

        login_server_send_packet(session->socket, response);
}

static void handle_login_server(session_t *session)
{
        packet_t response[16] = { 0 };

        packet_play_ok_t play_ok = { 0 };

        login_server_log("Player wants to log into game server.");

        play_ok.playOK1 = session->playOK1;
        play_ok.playOK2 = session->playOK2;

        packet_play_ok_pack(response, &play_ok);
        login_server_encrypt(session->blowfish, response, response);

        login_server_send_packet(session->socket, response);
}

void login_server_request_new_conn(socket_t *socket)
{
        static packet_t      response[256] = { 0 };
        static byte_t        modulus[128]  = { 0 };
        static packet_init_t init          = { 0 };

        session_t *session = 0;

        bytes_zero(response, sizeof(response));
        bytes_zero(modulus, sizeof(modulus));
        bytes_zero((byte_t *) &init, sizeof(init));

        session = session_new(socket);

        rsa_modulus(session->rsa, modulus);
        packet_init(&init, modulus);
        packet_init_pack(response, &init);

        login_server_send_packet(socket, response);
}

void login_server_request(socket_t *socket, byte_t *buf, size_t n)
{
        // 65536 being the limit for a single packet.
        static packet_t packet[65536] = { 0 };

        session_t *session = 0;

        u16_t size = 0;

        session = session_find(socket);

        if (!session) {
                login_server_log("Session not found. Ignoring request.");
                return;
        }

        bytes_zero(packet, sizeof(packet));
        login_server_decrypt(session->blowfish, packet, buf);

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
                login_server_log("Ignoring unknown packet.");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                login_server_request(socket, buf + size, n - size);
        }
}

void login_server_request_disconnect(socket_t *socket)
{
        PREVENT_UNUSED_WARNING(socket);
}
