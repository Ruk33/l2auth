#include "../headers.h"
#include "../session.h"
#include "../server_packet/validate_position.h"
#include "../character.h"
#include "validate_position.h"

void client_request_validate_position(
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

        struct ClientRequestValidatePosition request_packet = {0};
        byte *p = packet_body(request);

        packet response[SERVER_PACKET_VALIDATE_POSITION_FULL_SIZE] = {0};

        BYTE_READ_VAL(request_packet.position.x, p);
        BYTE_READ_VAL(request_packet.position.y, p);
        BYTE_READ_VAL(request_packet.position.z, p);
        BYTE_READ_VAL(request_packet.heading, p);

        character_validate_position(
                character,
                &request_packet.position,
                request_packet.heading
        );

        server_packet_validate_position(
                response,
                character,
                request_packet.heading
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
}
