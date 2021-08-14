#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/list.h"
#include "include/recycle_id.h"
#include "include/os_io.h"
#include "include/l2_string.h"
#include "include/gs_types.h"
#include "include/gs_crypt.h"
#include "include/gs_client_packets.h"
#include "include/gs_random_id.h"
#include "include/gs_session.h"

#define gs_session_each(session, state) \
        list_each(struct gs_session, session, state->list_sessions)

static size_t gs_session_get_free_id(struct gs_state *state)
{
        // Don't use id 0, it causes issues with packets
        // sent to the client.
        for (size_t i = 1, max = arr_size(state->sessions); i < max; i += 1) {
                if (!state->sessions[i].id) {
                        return i;
                }
        }

        return 0;
}

struct gs_session *gs_session_new(struct gs_state *state, struct os_io *socket)
{
        byte_t key[] = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };

        size_t id = 0;

        struct gs_session *new_session = 0;

        assert(socket);
        assert(state);

        id = gs_session_get_free_id(state);

        new_session = &state->sessions[id];

        bytes_zero((byte_t *) new_session, sizeof(*new_session));

        new_session->id     = (u32_t) id;
        new_session->socket = socket;

        bytes_cpy(new_session->encrypt_key, key, sizeof(key));
        bytes_cpy(new_session->decrypt_key, key, sizeof(key));

        list_add(state->list_sessions, new_session);

        return new_session;
}

struct gs_session *gs_session_find(struct gs_state *state, struct os_io *socket)
{
        struct gs_session *session = 0;

        assert(state);
        assert(socket);

        gs_session_each(session, state)
        {
                if (session->socket == socket) {
                        return session;
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
        struct gs_packet_auth_request *src)
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

void gs_session_disconnect(struct gs_state *state, struct gs_session *session)
{
        assert(state);
        assert(session);

        list_remove(state->list_sessions, session);
        *session = (struct gs_session){ 0 };
}
