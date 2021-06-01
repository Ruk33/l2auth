#include "include/util.h"
#include "include/config.h"
#include "include/socket.h"
#include "include/blowfish.h"
#include "include/rsa.h"
#include "include/session.h"

static session_t sessions[MAX_CLIENTS] = { 0 };
static size_t    session_count         = 0;

session_t *session_new(socket_t *socket)
{
        session_t *session = 0;

        session = &sessions[session_count];

        session->socket        = socket;
        session->rsa_decrypted = 0;

        session->rsa      = rsa_new();
        session->blowfish = blowfish_new();

        session_count += 1;

        return session;
}

session_t *session_find(socket_t *socket)
{
        for (size_t i = 0; i < session_count; i += 1) {
                if (sessions[i].socket == socket) {
                        return &sessions[i];
                }
        }

        return 0;
}
