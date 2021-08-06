#include <assert.h>
#include "include/util.h"
#include "include/recycle_id.h"
#include "include/config.h"
#include "include/os_io.h"
#include "include/ls_blowfish.h"
#include "include/ls_rsa.h"
#include "include/ls_session.h"

static ls_session_t sessions[MAX_CLIENTS]    = { 0 };
static size_t session_instances[MAX_CLIENTS] = { 0 };
static size_t session_count                  = 0;

ls_session_t *ls_session_new(struct os_io *socket)
{
        ls_session_t *session = 0;

        size_t id = 0;

        assert(socket);
        assert(session_count < arr_size(sessions));

        session_count += recycle_id_get(&id, session_instances);
        session = &sessions[id];

        session->socket   = socket;
        session->rsa      = ls_rsa_new();
        session->blowfish = ls_blowfish_new();

        assert(session->rsa);
        assert(session->blowfish);

        return session;
}

ls_session_t *ls_session_find(struct os_io *socket)
{
        assert(socket);

        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}
