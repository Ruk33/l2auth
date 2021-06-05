#include "include/util.h"
#include "include/config.h"
#include "include/os_socket.h"
#include "include/ls_blowfish.h"
#include "include/ls_rsa.h"
#include "include/ls_session.h"

static ls_session_t sessions[MAX_CLIENTS] = { 0 };
static size_t session_count               = 0;

ls_session_t *ls_session_new(os_socket_t *socket)
{
        ls_session_t *session = 0;

        session = &sessions[session_count];

        session->socket   = socket;
        session->rsa      = ls_rsa_new();
        session->blowfish = ls_blowfish_new();

        session_count += 1;

        return session;
}

ls_session_t *ls_session_find(os_socket_t *socket)
{
        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}
