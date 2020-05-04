#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <login/dto/session_key.h>
#include <game/packet/char_list.h>
#include <game/handler/encrypt.h>
#include <game/handler/auth_login.h>

void game_handler_auth_login
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);

        struct LoginDtoSessionKey* session = l2_client_session(client);
        assert(session);

        size_t login_len = l2_string_len(request + 3) + 1;
        size_t login_name_as_string_len =
                l2_string_calculate_space_from_char(login_len);
        l2_string* login_name_as_string =
                l2_client_alloc_temp_mem(client, login_name_as_string_len);

        l2_string_from_l2(login_name_as_string, request + 3, login_name_as_string_len);
        l2_string_to_char(login_name_as_string, session->login_name, login_len);
        log_info("Login name %s", session->login_name);

        memcpy(
                &session->playOK2,
                request + 3 + login_name_as_string_len,
                sizeof(int)
        );
        memcpy(
                &session->playOK1,
                request + 3 + login_name_as_string_len + sizeof(int),
                sizeof(int)
        );
        memcpy(
                &session->loginOK1,
                request + 3 + login_name_as_string_len + sizeof(int) * 2,
                sizeof(int)
        );
        memcpy(
                &session->loginOK2,
                request + 3 + login_name_as_string_len + sizeof(int) * 3,
                sizeof(int)
        );

        log_info("Session key - Play OK 1 %d", session->playOK1);
        log_info("Session key - Play OK 2 %d", session->playOK2);
        log_info("Session key - Login OK 1 %d", session->loginOK1);
        log_info("Session key - Login OK 2 %d", session->loginOK2);

        l2_packet* response = game_packet_char_list(client);

        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}
