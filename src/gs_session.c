#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/os_io.h"
#include "include/l2_string.h"
#include "include/gs_types.h"
#include "include/gs_crypt.h"
#include "include/gs_packet_auth_request.h"
#include "include/gs_random_id.h"
#include "include/gs_session.h"

static struct gs_session *sessions = 0;
static size_t *session_count       = 0;

void gs_session_set(struct gs_session *src, size_t *count)
{
        sessions      = src;
        session_count = count;
}

struct gs_session *gs_session_new(os_io_t *socket)
{
        byte_t key[] = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };
        struct gs_session *new_session = 0;

        assert(socket);
        assert(*session_count < MAX_CLIENTS);

        new_session = &sessions[*session_count];

        bytes_zero((byte_t *) new_session, sizeof(*new_session));

        new_session->socket = socket;

        gs_random_id(&new_session->id);
        bytes_cpy(new_session->encrypt_key, key, sizeof(key));
        bytes_cpy(new_session->decrypt_key, key, sizeof(key));

        *session_count += 1;

        return new_session;
}

struct gs_session *gs_session_find(os_io_t *socket)
{
        assert(socket);

        for (size_t i = 0; i < *session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}

void gs_session_encrypt_conn(struct gs_session *session)
{
        assert(session);
        session->conn_encrypted = 1;
}

void gs_session_update_auth(
        struct gs_session *dest,
        gs_packet_auth_request_t *src)
{
        size_t max_username_size = 0;

        assert(dest);
        assert(src);

        max_username_size = sizeof(dest->username);
        l2_string_to_char(dest->username, src->username, max_username_size);

        dest->loginOK1 = src->loginOK1;
        dest->loginOK2 = src->loginOK2;
        dest->playOK1  = src->playOK1;
        dest->playOK2  = src->playOK2;
}

void gs_session_encrypt(struct gs_session *session, byte_t *dest, packet_t *src)
{
        assert(session);
        assert(dest);
        assert(src);
        gs_encrypt(session->encrypt_key, dest, src);
}

void gs_session_decrypt(struct gs_session *session, packet_t *dest, byte_t *src)
{
        assert(session);
        assert(dest);
        assert(src);

        if (session->conn_encrypted) {
                gs_decrypt(session->decrypt_key, dest, src);
                return;
        }

        bytes_cpy(dest, src, (size_t) packet_size(src));
}
