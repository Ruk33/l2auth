#include <assert.h>
#include <math.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include "../dto/character.h"
#include "../request_handler/action/my_target_selected_response.h"
#include "../request_handler/action/action_fail_response.h"
#include "../request_handler/say/response.h"
#include "../client.h"
#include "../dto/pc.h"

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

        struct Pc *player = NULL;
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

        if (d > 300) player_unsafe_say(client, "Will test one more time, if it works, nice!");

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
