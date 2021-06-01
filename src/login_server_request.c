#include <stdio.h>
#include "include/util.h"
#include "include/socket.h"
#include "include/rsa.h"
#include "include/packet.h"
#include "include/session.h"
#include "include/packet_init.h"
#include "include/packet_gg_auth.h"
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
        static packet_t packet[65536] = { 0 };

        session_t *session = 0;

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
                login_server_log("Handle login server");
                break;
        case 0x05: // Request server list
                login_server_log("Handle request server list");
                break;
        case 0x07: // GG Auth
                handle_gg_auth(session);
                break;
        default:
                login_server_log("Packet what?: %d", packet_type(packet));
                break;
        }

        // There can be multiple packets inside of buf.
        if (packet_size(buf) < n) {
                login_server_request(
                        socket, buf + packet_size(buf), n - packet_size(buf));
        }
}

void login_server_request_disconnect(socket_t *socket)
{
        PREVENT_UNUSED_WARNING(socket);
}
