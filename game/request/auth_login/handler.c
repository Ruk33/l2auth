#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_auth_login_handler(struct GameRequest* request)
{
        assert(request);

        struct GameClient* server = request->conn->server;
        struct GameClient* client = request->conn->client;
        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct L2SessionKey* session = game_client_session(client);
        unsigned char* content = l2_packet_content(request->packet);

        size_t login_len = l2_string_len((l2_string *) content) + 1;
        size_t login_name_str_len = l2_string_calculate_space_from_char(login_len);
        l2_string* login_name_str = game_client_alloc_temp_mem(client, login_name_str_len);

        l2_string_from_l2(login_name_str, content, login_name_str_len);
        l2_string_to_char(login_name_str, session->login_name, login_len);

        log_info("Login name %s", session->login_name);

        content += login_name_str_len;

        content = byte_reader_cpy_int_n_mv(content, &session->playOK2);
        content = byte_reader_cpy_int_n_mv(content, &session->playOK1);
        content = byte_reader_cpy_int_n_mv(content, &session->loginOK1);
        content = byte_reader_cpy_int_n_mv(content, &session->loginOK2);

        log_info("Session key - Play OK 1 %d", session->playOK1);
        log_info("Session key - Play OK 2 %d", session->playOK2);
        log_info("Session key - Login OK 1 %d", session->loginOK1);
        log_info("Session key - Login OK 2 %d", session->loginOK2);

        l2_packet* response = game_request_auth_login_response(client);

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        request->conn->handler = game_request_auth_login_next_handler;
}
