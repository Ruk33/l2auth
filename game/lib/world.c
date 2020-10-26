#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <data_structure/list.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "host.h"
#include "client.h"
#include "request_handler/protocol_version/handler.h"
#include "request_handler/move/handler.h"
#include "request_handler/action/handler.h"
#include "request_handler/auth_login/handler.h"
#include "request_handler/select_character/handler.h"
#include "request_handler/d0/handler.h"
#include "request_handler/quest_list/handler.h"
#include "request_handler/enter_world/handler.h"
#include "request_handler/restart/handler.h"
#include "request_handler/validate_position/handler.h"
#include "request_handler/say/handler.h"
#include "request_handler/logout/handler.h"
#include "request_handler/create_char/handler.h"
#include "request_handler/new_char/handler.h"
#include "request_handler/show_map/handler.h"
#include "request_handler/type.h"
#include "world_state.h"
#include "request.h"
#include "world.h"

struct World
{
        host_malloc_cb memory_alloc;
        host_mfree_cb memory_free;
        host_send_response_cb send_response;

        struct WorldState *state;
};

void *world_new(host_malloc_cb m, host_mfree_cb f, host_send_response_cb s)
{
        struct World *world = NULL;

        world = m(sizeof(*world));
        world->memory_alloc = m;
        world->memory_free = f;
        world->send_response = s;
        world->state = world_state_new(m, f);

        return world;
}

void world_new_client(void *world_p, int client_id)
{
        struct World *world = NULL;
        struct Client *client = NULL;

        world = world_p;
        client = client_new(
            client_id,
            world->memory_alloc,
            world->memory_free,
            world->send_response);

        world_state_spawn_client(world->state, client);
}

static void world_handle_client_request(struct World *world, struct Client *client, l2_raw_packet *packet)
{
        assert(world);
        assert(client);
        assert(packet);

        struct Request request;

        request.world_state = world->state;
        request.client = client;
        request.packet = packet;

        if (client_is_conn_encrypted(client))
                client_decrypt_packet(client, packet);

        client_encrypt_conn(client);
        log_info("Packet type %02X", l2_packet_get_type(packet));

        switch (l2_packet_get_type(packet))
        {
        case REQUEST_TYPE_PROTOCOL_VERSION:
                protocol_version_handler(&request);
                break;
        case REQUEST_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                move_handler(&request);
                break;
        case REQUEST_TYPE_ACTION:
                action_handler(&request);
                break;
        case REQUEST_TYPE_AUTH_REQUEST:
                auth_login_handler(&request);
                break;
        case REQUEST_TYPE_NEW_CHAR:
                new_char_handler(&request);
                break;
        case REQUEST_TYPE_CREATE_CHAR:
                create_char_handler(&request);
                break;
        case REQUEST_TYPE_SELECTED_CHAR:
                select_character_handler(&request);
                break;
        case REQUEST_TYPE_REQUEST_AUTO_SS_BSPS:
                d0_handler(&request);
                break;
        case REQUEST_TYPE_REQUEST_QUEST_LIST:
                quest_list_handler(&request);
                break;
        case REQUEST_TYPE_ENTER_WORLD:
                enter_world_handler(&request);
                break;
        case REQUEST_TYPE_RESTART:
                restart_handler(&request);
                break;
        case REQUEST_TYPE_VALIDATE_POS:
                validate_position_handler(&request);
                break;
        case REQUEST_TYPE_SAY:
                say_handler(&request);
                break;
        case REQUEST_TYPE_LOGOUT:
                logout_handler(&request);
                break;
        case REQUEST_TYPE_SHOW_MAP:
                show_map_handler(&request);
                break;
        default:
                break;
        }
}

void world_client_request(void *world_p, int client_id, unsigned char *buf, size_t buf_size)
{
        assert(buf);
        assert(buf_size > 0);

        struct World *world = NULL;
        struct Client *client = NULL;
        l2_raw_packet *raw_packet = NULL;
        unsigned char *packet_content = NULL;
        unsigned short packet_size = 0;
        l2_raw_packet *packet = NULL;

        world = world_p;
        client = world_state_find_client(world->state, client_id);

        if (!client)
        {
                log_fatal("Client with id %d not found, ignoring request", client_id);
                return;
        }

        raw_packet = (l2_raw_packet *)buf;
        packet_content = l2_raw_packet_content(raw_packet);
        packet_size = (unsigned short)(buf_size - 2);
        packet = world->memory_alloc(l2_raw_packet_calculate_size((unsigned short)buf_size));

        l2_raw_packet_init(packet, packet_content, packet_size);
        world_handle_client_request(world, client, packet);

        world->memory_free(packet);
}

void world_client_disconnect(void *world_p, int client_id)
{
        struct World *world = NULL;
        struct Client *client = NULL;

        world = world_p;
        client = world_state_find_client(world->state, client_id);

        if (!client)
        {
                log_fatal(
                    "Client with id %d not found, ignoring disconnect",
                    client_id);
                return;
        }

        world_state_unspawn_client(world->state, client);
}
