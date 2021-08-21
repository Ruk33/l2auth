#include <assert.h>
#include "include/os_io.h"
#include "include/config.h"
#include "include/util.h"
#include "include/conn.h"
#include "include/log.h"
#include "include/packet.h"
#include "include/server.h"
#include "include/ls_session.h"
#include "include/ls_server_packets.h"
#include "include/ls_request.h"

static void handle_gg_auth(struct ls_session *session)
{
        packet_t response[16] = { 0 };

        struct ls_packet_gg_auth gg_auth = { 0 };

        assert(session);

        gg_auth.response = PACKET_GG_AUTH_RESPONSE_SKIP;

        ls_packet_gg_auth_pack(response, &gg_auth);
        ls_session_encrypt_packet(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_auth_login(struct ls_session *session)
{
        packet_t response[64] = { 0 };

        struct ls_packet_ok ok = { 0 };

        assert(session);

        // Todo: Generate these values randomly.
        session->playOK1 = 42;
        session->playOK2 = 24;
        ok.loginOK1      = session->playOK1;
        ok.loginOK2      = session->playOK2;

        ls_packet_ok_pack(response, &ok);
        ls_session_encrypt_packet(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_request_server_list(struct ls_session *session)
{
        static packet_t response[512] = { 0 };

        static struct ls_packet_server_list server_list = { 0 };

        server_t *gs_servers = 0;

        assert(session);

        gs_servers = server_all();
        assert(gs_servers);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &server_list, sizeof(server_list));

        server_list.count = server_all_count();
        assert(server_list.count <= arr_size(server_list.servers));

        for (u8_t i = 0; i < server_list.count; i += 1) {
                server_list.servers[i] = gs_servers[i];
        }

        ls_packet_server_list_pack(response, &server_list);
        ls_session_encrypt_packet(session, response, response);

        conn_send_packet(session->socket, response);
}

static void handle_login_server(struct ls_session *session)
{
        packet_t response[32] = { 0 };

        struct ls_packet_play_ok play_ok = { 0 };

        assert(session);

        log("player wants to log into game server.");

        play_ok.playOK1 = session->playOK1;
        play_ok.playOK2 = session->playOK2;

        ls_packet_play_ok_pack(response, &play_ok);
        ls_session_encrypt_packet(session, response, response);

        conn_send_packet(session->socket, response);
}

void ls_request_new_conn(struct os_io *socket)
{
        static packet_t response[256]     = { 0 };
        static struct ls_packet_init init = { 0 };

        // Todo: re-check from where does this session id comes from.
        byte_t session_id[] = { 0xfd, 0x8a, 0x22, 0x00 };

        // Chronicle 4 protocol only :)
        byte_t protocol[] = { 0x5a, 0x78, 0x00, 0x00 };

        struct ls_session *session = 0;

        assert(socket);

        bytes_zero(response, sizeof(response));
        bytes_zero((byte_t *) &init, sizeof(init));

        session = ls_session_new(socket);
        assert(session);

        bytes_cpy(init.session_id, session_id, sizeof(init.session_id));
        bytes_cpy(init.protocol, protocol, sizeof(init.protocol));
        ls_session_rsa_modulus(session, init.modulus);

        ls_packet_init_pack(response, &init);

        conn_send_packet(socket, response);
}

void ls_request(struct os_io *socket, byte_t *buf, size_t n)
{
        static packet_t packet[2048] = { 0 };

        struct ls_session *session = 0;

        u16_t size = 0;

        assert(socket);

        session = ls_session_find(socket);

        if (!session) {
                log("session not found. ignoring request.");
                return;
        }

        bytes_zero(packet, sizeof(packet));
        ls_session_decrypt_packet(session, packet, buf);

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
                log("ignoring unknown packet.");
                break;
        }

        size = packet_size(buf);

        // There can be multiple packets inside of buf.
        if (size < n) {
                ls_request(socket, buf + size, n - size);
        }
}

void ls_request_disconnect(struct os_io *socket)
{
        assert(socket);
        log("client disconnected from login server.");
}
