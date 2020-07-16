#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_reader.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/create.h>
#include <game/entity/player.h>
#include <game/entity/character.h>
#include <game/service/crypt/packet/encrypt.h>
#include <game/request/auth_login/handler.h>
#include "next_handler.h"
#include "response_ok.h"
#include "handler.h"

void game_handler_create_character_and_persist_into_db
(
        struct GameClient* client,
        l2_raw_packet* request
)
{
        assert(client);
        assert(request);

        struct Player player;
        size_t max_name_len = sizeof(player.character.name);
        l2_string* l2_name = game_client_alloc_temp_mem(
                client,
                l2_string_calculate_space_from_char(max_name_len)
        );
        unsigned char* content = l2_packet_content(request);

        l2_string_from_l2(l2_name, (l2_string *) content, max_name_len);
        l2_string_to_char(l2_name, player.character.name, max_name_len);

        content += l2_string_calculate_space_from_char(strlen(player.character.name) + 1);

        content = byte_reader_cpy_int_n_mv(content, &player.race_id);
        content = byte_reader_cpy_int_n_mv(content, &player.character.sex);
        content = byte_reader_cpy_int_n_mv(content, &player.class_id);
        content = byte_reader_cpy_int_n_mv(content, &player.character._int);
        content = byte_reader_cpy_int_n_mv(content, &player.character.str);
        content = byte_reader_cpy_int_n_mv(content, &player.character.con);
        content = byte_reader_cpy_int_n_mv(content, &player.character.men);
        content = byte_reader_cpy_int_n_mv(content, &player.character.dex);
        content = byte_reader_cpy_int_n_mv(content, &player.character.wit);
        content = byte_reader_cpy_int_n_mv(content, &player.hair_style_id);
        content = byte_reader_cpy_int_n_mv(content, &player.hair_color_id);
        content = byte_reader_cpy_int_n_mv(content, &player.face);

        game_service_character_create(&player);
}

void game_request_create_character_handler(struct GameRequest* request)
{
        assert(request);
        assert(request->conn);
        assert(request->packet);
        assert(request->conn->encrypt_key);
        assert(request->conn->client);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        // struct GameServer* server = request->conn->server;
        struct GameClient* client = request->conn->client;
        l2_packet* response = game_request_character_create_response_ok(client);
        
        game_handler_create_character_and_persist_into_db(
                client,
                request->packet
        );

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        game_request_auth_login_handler(request);

        request->conn->handler = game_request_create_character_next_handler;
}
