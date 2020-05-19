#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/character/load.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_select_character_handler(struct GameRequest* request)
{
        assert(request);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct GameClient* client = request->conn->client;
        struct GameEntityCharacter* character = game_client_get_char(client);
        unsigned char* content = l2_packet_content(request->packet);
        int selected_index = 0;
        l2_packet* response;

        byte_reader_cpy_int_n_mv(content, &selected_index);
        log_info("Selected char index %d", selected_index);

        game_service_character_load(selected_index, character);
        log_info("Information about selected char", character->name);
        log_info("Name %s", character->name);
        log_info("Char id %d", character->char_id);
        log_info("Race id %d", character->race_id);
        log_info("Class id %d", character->class_id);
        log_info("Sex %d", character->sex);
        log_info("Int %d", character->_int);
        log_info("Str %d", character->str);
        log_info("Con %d", character->con);
        log_info("Men %d", character->men);
        log_info("Dex %d", character->dex);
        log_info("Wit %d", character->wit);
        log_info("X %d", character->current_location.x);
        log_info("Y %d", character->current_location.y);
        log_info("Z %d", character->current_location.z);
        log_info("Current hp %f", character->current_hp);
        log_info("Current mp %f", character->current_mp);
        log_info("Max hp %f", character->max_hp);
        log_info("Max mp %f", character->max_mp);
        log_info("Level %d", character->level);
        log_info("Hair style %d", character->hair_style);
        log_info("Hair color %d", character->hair_color);
        log_info("Face ID %d", character->face);

        response = game_request_select_character_response(client, character);
        
        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        request->conn->handler = game_request_select_character_next_handler;
}
