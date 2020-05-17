#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include "response.h"
#include "handler.h"

void game_request_auth_login_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);

        struct L2SessionKey* session = game_client_session(client);
        unsigned char* request_content = l2_packet_content(request);

        size_t login_len = l2_string_len((l2_string *) request_content) + 1;
        size_t login_name_as_string_len =
                l2_string_calculate_space_from_char(login_len);
        l2_string* login_name_as_string =
                game_client_alloc_temp_mem(client, login_name_as_string_len);

        l2_string_from_l2(
                login_name_as_string,
                request_content,
                login_name_as_string_len
        );

        l2_string_to_char(
                login_name_as_string,
                session->login_name,
                login_len
        );

        log_info("Login name %s", session->login_name);

        request_content += login_name_as_string_len;

        request_content = byte_reader_copy_and_move(
                request_content,
                &session->playOK2,
                sizeof(int)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &session->playOK1,
                sizeof(int)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &session->loginOK1,
                sizeof(int)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &session->loginOK2,
                sizeof(int)
        );

        log_info("Session key - Play OK 1 %d", session->playOK1);
        log_info("Session key - Play OK 2 %d", session->playOK2);
        log_info("Session key - Login OK 1 %d", session->loginOK1);
        log_info("Session key - Login OK 2 %d", session->loginOK2);

        l2_packet* response = game_request_auth_login_response(client);

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );
}
