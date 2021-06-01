#include "include/config.h"
#include "include/util.h"
#include "include/socket.h"
#include "include/l2_string.h"
#include "include/game_server_crypt.h"
#include "include/packet_auth_request.h"
#include "include/gs_session.h"

static gs_session_t sessions[MAX_CLIENTS] = { 0 };
static size_t       session_count         = 0;

gs_session_t *gs_session_new(socket_t *socket)
{
        byte_t key[] = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };
        gs_session_t *new_session = 0;

        new_session = &sessions[session_count];

        bytes_zero((byte_t *) new_session, sizeof(*new_session));
        bytes_cpy(new_session->encrypt_key, key, sizeof(key));
        bytes_cpy(new_session->decrypt_key, key, sizeof(key));

        new_session->socket = socket;
        session_count += 1;

        return new_session;
}

gs_session_t *gs_session_find(socket_t *socket)
{
        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}

void gs_session_encrypt_conn(gs_session_t *session)
{
        session->conn_encrypted = 1;
}

void gs_session_update_auth(gs_session_t *dest, packet_auth_request_t *src)
{
        size_t max_username_size = 0;

        max_username_size = sizeof(dest->username);
        l2_string_to_char(dest->username, src->username, max_username_size);

        dest->loginOK1 = src->loginOK1;
        dest->loginOK2 = src->loginOK2;
        dest->playOK1  = src->playOK1;
        dest->playOK2  = src->playOK2;
}

void gs_session_encrypt(gs_session_t *session, byte_t *dest, packet_t *src)
{
        game_server_encrypt(session->encrypt_key, dest, src);
}

void gs_session_decrypt(gs_session_t *session, packet_t *dest, byte_t *src)
{
        if (session->conn_encrypted) {
                game_server_decrypt(session->decrypt_key, dest, src);
                return;
        }

        bytes_cpy(dest, src, (size_t) packet_size(src));
}
