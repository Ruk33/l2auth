#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_reader.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/create.h>
#include <game/entity/character.h>
#include <game/service/crypt/packet/encrypt.h>
#include <game/request/auth_login/handler.h>
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

        request = request + 3;

        l2_string_from_l2(l2_name, (l2_string *) request, max_name_len);
        l2_string_to_char(l2_name, character.name, max_name_len);

        request = (
                request +
                l2_string_calculate_space_from_char(strlen(character.name) + 1)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.race_id,
                sizeof(character.race_id)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.sex,
                sizeof(character.sex)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.class_id,
                sizeof(character.class_id)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character._int,
                sizeof(character._int)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.str,
                sizeof(character.str)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.con,
                sizeof(character.con)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.men,
                sizeof(character.men)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.dex,
                sizeof(character.dex)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.wit,
                sizeof(character.wit)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.hair_style,
                sizeof(character.hair_style)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.hair_color,
                sizeof(character.hair_color)
        );

        request = byte_reader_cpy_n_mv(
                request,
                &character.face,
                sizeof(character.face)
        );

        game_service_character_create(&character);
}

void game_request_create_character_handler
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
        assert(encrypt_key);

        l2_packet* response = game_request_character_create_response_ok(client);
        
        game_handler_create_character_and_persist_into_db(
                client,
                request
        );

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        game_request_auth_login_handler(server, client, request, encrypt_key);
}
