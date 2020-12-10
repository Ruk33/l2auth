#include "headers.h"
#include "encrypt_packet.h"
#include "decrypt_packet.h"
#include "session.h"

void session_encrypt_connection(session_t *session)
{
        assert(session);
        session->conn_encrypted = 1;
}

void session_encrypt_packet(session_t *session, byte *dest, packet *src, size_t src_size)
{
        assert(session);
        assert(dest);
        assert(src);
        assert(src_size);
        encrypt_packet(dest, src, src_size, session->encrypt_key);
}

void session_decrypt_packet(session_t *session, byte *dest, packet *src, size_t src_size)
{
        assert(session);
        assert(dest);
        assert(src);
        assert(src_size);

        if (!session->conn_encrypted)
        {
                memcpy(dest, src, src_size);
                return;
        }

        decrypt_packet(dest, src, src_size, session->decrypt_key);
}

void session_entered_world(session_t *session)
{
        assert(session);
        session->in_world = 1;
}

void session_leaved_world(session_t *session)
{
        assert(session);
        session->in_world = 0;
}

void session_update(
        session_t *session,
        char *username,
        size_t username_size,
        int loginOK1,
        int loginOK2,
        int playOK1,
        int playOK2
)
{
        assert(session);
        assert(username);
        assert(username_size);

        memset(session->username, 0, sizeof(session->username));
        memcpy(session->username, username, username_size);

        session->loginOK1 = loginOK1;
        session->loginOK2 = loginOK2;
        session->playOK1 = playOK1;
        session->playOK2 = playOK2;
}

void session_print(session_t *session)
{
        assert(session);
        printf("Username: %s.\n", session->username);
        printf("Play OK 1: %d.\n", session->playOK1);
        printf("Play OK 2: %d.\n", session->playOK2);
        printf("Login OK 1: %d.\n", session->loginOK1);
        printf("Login OK 2: %d.\n", session->loginOK2);
}
