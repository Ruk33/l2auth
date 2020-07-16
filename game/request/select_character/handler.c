#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_packet.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include <game/entity/player.h>
#include <game/service/character/load.h>
#include <game/service/crypt/packet/encrypt.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_select_character_handler(struct GameRequest* request)
{
        assert(request);
        assert(request->conn);
        assert(request->packet);
        assert(request->conn->encrypt_key);
        assert(request->conn->client);

        unsigned char* encrypt_key = request->conn->encrypt_key;
        struct GameClient* client = request->conn->client;
        struct Player* player = game_client_get_char(client);
        unsigned char* content = l2_packet_content(request->packet);
        int selected_index = 0;
        l2_packet* response;

        byte_reader_cpy_int_n_mv(content, &selected_index);
        log_info("Selected char index %d", selected_index);

        game_service_character_load(selected_index, player);
        log_info("Information about selected char", player->character.name);
        log_info("Name %s", player->character.name);
        log_info("Char id %d", player->character.id);
        log_info("Race id %d", player->race_id);
        log_info("Class id %d", player->class_id);
        log_info("Sex %d", player->character.sex);
        log_info("Int %d", player->character._int);
        log_info("Str %d", player->character.str);
        log_info("Con %d", player->character.con);
        log_info("Men %d", player->character.men);
        log_info("Dex %d", player->character.dex);
        log_info("Wit %d", player->character.wit);
        log_info("X %d", player->character.x);
        log_info("Y %d", player->character.y);
        log_info("Z %d", player->character.z);
        log_info("Current hp %f", player->character.current_hp);
        log_info("Current mp %f", player->character.current_mp);
        log_info("Max hp %f", player->character.hp);
        log_info("Max mp %f", player->character.mp);
        log_info("Level %d", player->character.level);
        log_info("Hair style %d", player->hair_style_id);
        log_info("Hair color %d", player->hair_color_id);
        log_info("Face ID %d", player->face);

        response = game_request_select_character_response(client, player);

        game_client_send_packet(
                client,
                game_service_crypt_packet_encrypt(response, encrypt_key)
        );

        request->conn->handler = game_request_select_character_next_handler;
}
