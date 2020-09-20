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
#include "../client.h"
#include "../dto/character.h"
#include "../dto/player.h"
#include "../storage/character.h"
#include "player.h"

void player_create
(struct Client *client, struct CreateCharRequest *request)
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
                l2_string_len(request->name)
        );

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
        player->character.x = -83968;                   // To be defined
        player->character.y = 244634;                   // To be defined
        player->character.z = -3730;                    // To be defined
        player->character.hp = 500;                     // To be defined
        player->character.mp = 500;                     // To be defined
        player->character.current_hp = 500;             // To be defined
        player->character.current_mp = 500;             // To be defined
        player->active = 1;
        player->character.level = 1;

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

void player_say
(struct Client *client, char *msg, size_t msg_len)
{
        assert(client);
        assert(msg);
        assert(msg_len > 0);

        l2_string *l2_message = NULL;
        size_t l2_message_size = 0;

        l2_packet *response = NULL;

        l2_message_size = l2_string_calculate_space_from_char(msg_len + 1);
        l2_message = client_alloc_mem(client, l2_message_size);

        l2_string_from_char(l2_message, msg, msg_len);

        response = say_response(client, l2_message);
        
        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, l2_message);
        client_free_mem(client, response);
}

void player_unsafe_say
(struct Client *client, char *msg)
{
        assert(client);
        assert(msg);
        player_say(client, msg, strlen(msg));
}

static void player_send_fail_action_response
(struct Client *client)
{
        assert(client);

        l2_packet *response = NULL;

        response = action_fail_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, response);
}

static void player_send_select_target_response
(struct Client *client, struct Character *target)
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

        if (d < 300) player_unsafe_say(client, "Selected unit is quite close");
        else player_unsafe_say(client, "Selected unit is far away");

        response = my_target_selected_response(client, target->id, 2);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        client_free_mem(client, player);
        client_free_mem(client, response);
}

void player_entity_action
(struct Client *client, struct Character *target)
{
        assert(client);

        if (target) {
                player_send_select_target_response(client, target);
                return;
        }

        player_send_fail_action_response(client);
}
