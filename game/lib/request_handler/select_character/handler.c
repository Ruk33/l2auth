#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../../storage/character.h"
#include "../../entity/player.h"
#include "../type.h"
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void select_character_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;

        unsigned char *content = NULL;

        int index = 0;
        struct Player *character = NULL;
        l2_packet *response = NULL;

        content = l2_packet_content(packet);

        byte_reader_cpy_int_n_mv(content, &index);
        log_info("Selected char with index %d.", index);

        character = storage_character_get(client, index);
        client_update_character(client, character);

        response = select_character_response(client);

        client_encrypt_packet(client, response);
        client_queue_response(client, response);

        // client_update_request_handler(client, &select_character_next_handler);

        client_free_mem(client, character);
        client_free_mem(client, response);
}
