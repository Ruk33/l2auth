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

        struct GameEntityCharacter character;
        size_t max_name_len = sizeof(character.name);
        l2_string* l2_name = game_client_alloc_temp_mem(
                client,
                l2_string_calculate_space_from_char(max_name_len)
        );
        unsigned char* content = l2_packet_content(request);

        l2_string_from_l2(l2_name, (l2_string *) request, max_name_len);
        l2_string_to_char(l2_name, character.name, max_name_len);

        content += l2_string_calculate_space_from_char(strlen(character.name) + 1);

        content = byte_reader_cpy_int_n_mv(content, &character.race_id);
        content = byte_reader_cpy_int_n_mv(content, &character.sex);
        content = byte_reader_cpy_int_n_mv(content, &character.class_id);
        content = byte_reader_cpy_int_n_mv(content, &character._int);
        content = byte_reader_cpy_int_n_mv(content, &character.str);
        content = byte_reader_cpy_int_n_mv(content, &character.con);
        content = byte_reader_cpy_int_n_mv(content, &character.men);
        content = byte_reader_cpy_int_n_mv(content, &character.dex);
        content = byte_reader_cpy_int_n_mv(content, &character.wit);
        content = byte_reader_cpy_int_n_mv(content, &character.hair_style);
        content = byte_reader_cpy_int_n_mv(content, &character.hair_color);
        content = byte_reader_cpy_int_n_mv(content, &character.face);

        game_service_character_create(&character);
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
