#include <assert.h>
#include <math.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include "../request_handler/create_char/request.h"
#include "../request_handler/action/my_target_selected_response.h"
#include "../request_handler/action/action_fail_response.h"
#include "../request_handler/say/response.h"
#include "../request_handler/create_char/response.h"
#include "../request_handler/auth_login/response.h"
#include "../request_handler/char_info/response.h"
#include "../request_handler/validate_position/response.h"
#include "../request_handler/move/response.h"
#include "../client.h"
#include "../dto/character.h"
#include "../dto/player.h"
#include "../storage/character.h"
#include "player.h"

void player_create(struct Client *client, struct CreateCharRequest *request)
{
        assert(client);
        assert(request);

        struct Player *player = NULL;
        l2_packet *creation_response = NULL;
        l2_packet *auth_response = NULL;

        player = client_alloc_mem(client, sizeof(*player));

        // Temp.
        player->character.id = rand();

        memset(player->character.name, 0, sizeof(player->character.name));
        l2_string_to_char(
            request->name,
            player->character.name,
            l2_string_len(request->name));

        player->race_id = request->race;
        player->character.sex = request->sex;
        player->class_id = request->class_id;
        player->character._int = request->_int;
        player->character.str = request->str;
        player->character.con = request->con;
        player->character.men = request->men;
        player->character.dex = request->dex;
        player->character.wit = request->wit;
        player->hair_style_id = request->hair_style;
        player->hair_color_id = request->hair_color;
        player->face = request->face;
        player->character.x = -83968;       // To be defined
        player->character.y = 244634;       // To be defined
        player->character.z = -3730;        // To be defined
        player->character.hp = 500;         // To be defined
        player->character.mp = 500;         // To be defined
        player->character.current_hp = 500; // To be defined
        player->character.current_mp = 500; // To be defined
        player->active = 1;
        player->character.level = 1;

        log_info("Player name: %s", player->character.name);
        log_info("Player race: %d", player->race_id);
        log_info("Player sex: %d", player->character.sex);
        log_info("Player class id: %d", player->class_id);
        log_info("Player int: %d", player->character._int);
        log_info("Player str: %d", player->character.str);
        log_info("Player con: %d", player->character.con);
        log_info("Player men: %d", player->character.men);
        log_info("Player dex: %d", player->character.dex);
        log_info("Player wit: %d", player->character.wit);
        log_info("Player hair style: %d", player->hair_style_id);
        log_info("Player face: %d", player->face);
        log_info("Player x: %d", player->character.x);
        log_info("Player y: %d", player->character.y);
        log_info("Player z: %d", player->character.z);
        log_info("Player hp: %d", player->character.hp);
        log_info("Player mp: %d", player->character.mp);

        storage_character_save(client, player);

        // Make validations, if everthing is ok then respond
        creation_response = create_char_response(client);
        auth_response = auth_login_response(client);

        client_encrypt_packet(client, creation_response);
        client_queue_response(client, creation_response);

        client_encrypt_packet(client, auth_response);
        client_queue_response(client, auth_response);

        client_free_mem(client, creation_response);
        client_free_mem(client, auth_response);
        client_free_mem(client, player);
}

void player_send_info(struct Client *from, struct Client *to)
{
        assert(from);
        assert(to);

        l2_packet *packet = NULL;

        if (!client_is_in_game(from))
                return;
        if (!client_is_in_game(to))
                return;

        packet = char_info_response(from);

        client_encrypt_packet(to, packet);
        client_queue_response(to, packet);

        client_free_mem(from, packet);
}

void player_validate_location_to(struct Client *from, struct Client *to, struct Vec3 *location, int heading)
{
        assert(from);
        assert(to);
        assert(location);

        struct Player *player = client_player(from);
        l2_packet *packet = validate_position_response(from, *location, heading);

        player->character.x = location->x;
        player->character.y = location->y;
        player->character.z = location->z;
        player->character.heading = heading;

        client_update_character(from, player);

        client_encrypt_packet(to, packet);
        client_queue_response(to, packet);

        client_free_mem(from, packet);
        client_free_mem(from, player);
}

void player_move_and_notify(struct Client *from, struct Client *to, struct Vec3 *prev_location, struct Vec3 *new_location)
{
        assert(from);
        assert(to);
        assert(prev_location);
        assert(new_location);

        l2_packet *response = NULL;

        response = move_response(from, *prev_location, *new_location);

        client_encrypt_packet(to, response);
        client_queue_response(to, response);

        client_free_mem(from, response);
}

void player_say(struct Client *from, struct Client *to, char *msg, size_t msg_len)
{
        assert(from);
        assert(to);
        assert(msg);
        assert(msg_len > 0);

        l2_string *l2_message = NULL;
        size_t l2_message_size = 0;

        l2_packet *response = NULL;

        l2_message_size = l2_string_calculate_space_from_char(msg_len + 1);
        l2_message = client_alloc_mem(from, l2_message_size);

        l2_string_from_char(l2_message, msg, msg_len);

        response = say_response(from, l2_message);

        client_encrypt_packet(to, response);
        client_queue_response(to, response);

        client_free_mem(from, l2_message);
        client_free_mem(from, response);
}

void player_unsafe_say(struct Client *client, const char *msg)
{
        assert(client);
        assert(msg);
        player_say(client, client, (char *)msg, strlen(msg));
}

static void player_send_fail_action_response(struct Client *client)
{
        assert(client);

        l2_packet *response = NULL;

        response = action_fail_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, response);
}

static void player_send_select_target_response(struct Client *client, struct Character *target)
{
        assert(client);
        assert(target);

        struct Player *player = NULL;
        struct Character player_character;

        double dx = 0;
        double dy = 0;
        double d = 0;

        l2_packet *response = NULL;

        player = client_player(client);
        player_character = player->character;

        dx = target->x - player_character.x;
        dy = target->y - player_character.y;
        d = sqrt(dx * dx + dy * dy);

        if (d < 300)
                player_unsafe_say(client, "Selected unit is quite close");
        else
                player_unsafe_say(client, "Selected unit is far away");

        response = my_target_selected_response(client, target->id, 2);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, player);
        client_free_mem(client, response);
}

void player_entity_action(struct Client *client, struct Character *target)
{
        assert(client);

        if (target)
        {
                player_send_select_target_response(client, target);
                return;
        }

        player_send_fail_action_response(client);
}
