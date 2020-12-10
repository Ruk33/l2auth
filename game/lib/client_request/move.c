#include "../headers.h"
#include "../session.h"
#include "../server_packet/move.h"
#include "../character.h"
#include "move.h"

void client_request_move(
        int client,
        packet *request,
        session_t *session,
        character_t *character,
        host_send_response_cb send_response
)
{
        assert(client > 0);
        assert(request);
        assert(session);
        assert(character);
        assert(send_response);

        byte *p = packet_body(request);
        struct ClientRequestMove request_packet = {0};

        packet response[SERVER_PACKET_MOVE_FULL_SIZE] = {0};

        BYTE_READ_VAL(request_packet.position.x, p);
        BYTE_READ_VAL(request_packet.position.y, p);
        BYTE_READ_VAL(request_packet.position.z, p);

        server_packet_move(
                response,
                character,
                &request_packet.position
        );
        session_encrypt_packet(
                session,
                response,
                response,
                (size_t) packet_get_size(response)
        );
        send_response(
                client,
                response,
                (size_t) packet_get_size(response)
        );

        character_move(character, &request_packet.position);
}
