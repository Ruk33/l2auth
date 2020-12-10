#include "../headers.h"
#include "../session.h"
#include "../server_packet/say.h"
#include "say.h"

void client_request_say(
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

        packet response[SERVER_PACKET_SAY_FULL_SIZE] = {0};
        l2_string *msg_from_client = packet_body(request);
        size_t msg_len = l2_string_len(msg_from_client) + 1;

        char msg[128] = {0};

        size_t safe_msg_size = msg_len < sizeof(msg) ? msg_len : sizeof(msg);

        l2_string_to_char(msg, msg_from_client, safe_msg_size);

        server_packet_say(
                response,
                character,
                SAY_MSG_TYPE_NORMAL,
                msg,
                strlen(msg) + 1
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
