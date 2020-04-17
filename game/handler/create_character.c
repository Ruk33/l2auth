#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_reader.h>
#include <game/service/create_char.h>
#include <game/packet/char_create_ok.h>
#include <game/dto/char.h>
#include <game/handler/encrypt.h>
#include <game/handler/auth_login.h>
#include <game/handler/create_character.h>

void game_handler_create_character_and_persist_into_db
(
        struct L2Client* client,
        l2_raw_packet* request
)
{
        assert(client);
        assert(request);

        struct GameDtoChar character;
        size_t max_name_len = sizeof(character.name);
        l2_string* l2_name = l2_client_alloc_temp_mem(
                client,
                l2_string_calculate_space_from_char(max_name_len)
        );

        request = request + 3;

        l2_string_from_l2(l2_name, (l2_string *) request, max_name_len);
        l2_string_to_char(l2_name, character.name, max_name_len);

        request = (
                request +
                // (franco.montenegro) + 1 (null character)
                l2_string_calculate_space_from_char(strlen(character.name) + 1)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.race_id,
                sizeof(character.race_id)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.sex,
                sizeof(character.sex)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.class_id,
                sizeof(character.class_id)
        );

        request = byte_reader_copy_and_move(
                request,
                &character._int,
                sizeof(character._int)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.str,
                sizeof(character.str)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.con,
                sizeof(character.con)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.men,
                sizeof(character.men)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.dex,
                sizeof(character.dex)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.wit,
                sizeof(character.wit)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.hair_style,
                sizeof(character.hair_style)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.hair_color,
                sizeof(character.hair_color)
        );

        request = byte_reader_copy_and_move(
                request,
                &character.face,
                sizeof(character.face)
        );

        game_service_create_char(&character);
}

void game_handler_create_character
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
        assert(encrypt_key);

        l2_packet* response = game_packet_char_create_ok(client);
        
        game_handler_create_character_and_persist_into_db(
                client,
                request
        );

        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );

        game_handler_auth_login(server, client, request, encrypt_key);
}
