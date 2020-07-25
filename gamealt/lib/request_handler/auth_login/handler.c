#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <core/l2_string.h>
#include <core/byte_reader.h>
#include "../../client.h"
#include "../type.h"
#include "../select_character/handler.h"
#include "response.h"
#include "handler.h"

void auth_login_handler(struct Client *client, l2_raw_packet *packet)
{
        l2_packet_type type = l2_packet_get_type(packet);
        unsigned char* content = l2_packet_content(packet);
        struct L2SessionKey session;

        size_t login_name_len;
        size_t login_name_size;
        l2_string *login_name;

        l2_packet *response;
        
        if (type != REQUEST_TYPE_AUTH_REQUEST) {
                log_fatal("Invalid request to be handled by auth login");
                return;
        }

        login_name_len = l2_string_len((l2_string *) content) + 1;
        login_name_size = l2_string_calculate_space_from_char(login_name_len);
        login_name = client_alloc_mem(client, login_name_size);

        l2_string_from_l2(login_name, (l2_string *) content, login_name_size);
        l2_string_to_char(login_name, (char *) session.login_name, login_name_len);
        log_info("Login name %s", session.login_name);

        content += login_name_size;

        content = byte_reader_cpy_int_n_mv(content, &session.playOK2);
        content = byte_reader_cpy_int_n_mv(content, &session.playOK1);
        content = byte_reader_cpy_int_n_mv(content, &session.loginOK1);
        content = byte_reader_cpy_int_n_mv(content, &session.loginOK2);

        log_info("Session key - Play OK 1 %d", session.playOK1);
        log_info("Session key - Play OK 2 %d", session.playOK2);
        log_info("Session key - Login OK 1 %d", session.loginOK1);
        log_info("Session key - Login OK 2 %d", session.loginOK2);

        client_update_session(client, &session);

        response = auth_login_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_update_request_handler(client, select_character_handler);

        client_free_mem(client, response);
        client_free_mem(client, login_name);
}
