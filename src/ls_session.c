#include <assert.h>
#include "include/util.h"
#include "include/config.h"
#include "include/os_io.h"
#include "include/ls_blowfish.h"
#include "include/ls_rsa.h"
#include "include/ls_session.h"

static ls_session_t sessions[MAX_CLIENTS] = { 0 };
static size_t session_count               = 0;

ls_session_t *ls_session_new(os_io_t *socket)
{
        ls_session_t *session = 0;

        assert(socket);
        assert(session_count < arr_size(sessions));

        session = &sessions[session_count];

        session->socket   = socket;
        session->rsa      = ls_rsa_new();
        session->blowfish = ls_blowfish_new();

        assert(session->rsa);
        assert(session->blowfish);

        session_count += 1;

        return session;
}

ls_session_t *ls_session_find(os_io_t *socket)
{
        assert(socket);

        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}
