#include "../headers.h"
#include "../storage/character.h"
#include "../storage/session.h"
#include "../server_packet/auth_login.h"
#include "auth_request.h"

static void update_session_from_packet(session_t *session, packet *src)
{
        assert(session);
        assert(src);

        byte *p = packet_body(src);

        char username[28] = {0};
        int playOK1 = 0;
        int playOK2 = 0;
        int loginOK1 = 0;
        int loginOK2 = 0;

        l2_string_to_char(username, p, sizeof(username));
        p += l2_string_bytes(p);

        BYTE_READ_VAL(playOK2, p);
        BYTE_READ_VAL(playOK1, p);
        BYTE_READ_VAL(loginOK1, p);
        BYTE_READ_VAL(loginOK2, p);

        session_update(
                session,
                username,
                strlen(username) + 1,
                loginOK1,
                loginOK2,
                playOK1,
                playOK2
        );
}

void client_request_auth_request(
        int client,
        packet *request,
        storage_character_t *character_storage,
        session_t *session,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(request);
        assert(session);
        assert(send_response);

        packet response[SERVER_PACKET_AUTH_LOGIN_FULL_SIZE] = {0};
        size_t response_size = 0;
        struct List *characters = NULL;

        update_session_from_packet(session, request);
        session_print(session);

        characters = storage_character_all_from_session(
                character_storage,
                session
        );

        server_packet_auth_login(response, session->playOK1, characters);
        response_size = (size_t) packet_get_size(response);

        session_encrypt_packet(session, response, response, response_size);
        send_response(client, response, response_size);
}

void client_request_auth_request_from_session(
        int client,
        session_t *session,
        storage_character_t *character_storage,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(session);
        assert(character_storage);
        assert(send_response);

        packet auth_login[SERVER_PACKET_AUTH_LOGIN_FULL_SIZE] = {0};
        struct List *characters = storage_character_all_from_session(
                character_storage,
                session
        );

        server_packet_auth_login(
                auth_login,
                session->playOK1,
                characters
        );
        session_encrypt_packet(
                session,
                auth_login,
                auth_login,
                (size_t) packet_get_size(auth_login)
        );
        send_response(
                client,
                auth_login,
                (size_t) packet_get_size(auth_login)
        );
}
