#include <request.h>
#include <util/session_crypt.h>
#include <db/conn.h>
#include <db/session.h>
#include "protocol_version.h"
#include "auth_request.h"
#include "character_selection.h"
#include "creating_character.h"
#include "entering_world.h"
#include "in_world.h"
#include "handle.h"

void state_machine_handle(
        int        client,
        byte_t *   request_packet,
        ssize_t    request_size,
        host_t *   host,
        db_conn_t *db)
{
        int       session_found    = 0;
        request_t request          = { 0 };
        packet *  decrypted_packet = NULL;
        session_t session          = { 0 };

        assert(request_packet);
        assert(host);
        assert(db);

        session_found = db_session_get(db, &session, client);

        if (!session_found) {
                printf("Warning: session for client %d not found.\n", client);
                printf("Ignoring request.\n");
                goto check_for_other_packets;
        }

        decrypted_packet = host->alloc_memory(65536);
        util_session_decrypt_packet(
                db,
                session.socket,
                decrypted_packet,
                request_packet,
                packet_get_size(request_packet));

        session_encrypt_connection(&session);
        util_session_encrypt_connection(db, session.socket);

        request.host    = host;
        request.session = &session;
        request.storage = db;
        request.packet  = decrypted_packet;
        request.size    = request_size;

        switch (session.state) {
        case PROTOCOL_VERSION:
                state_machine_protocol_version(&request);
                break;
        case AUTH_REQUEST:
                state_machine_auth_request(&request, 1);
                break;
        case CHARACTER_SELECTION:
                state_machine_character_selection(&request);
                break;
        case CREATING_CHARACTER:
                state_machine_creating_character(&request);
                break;
        case ENTERING_WORLD:
                state_machine_entering_world(&request);
                break;
        case IN_WORLD:
                state_machine_in_world(&request);
                break;
        default:
                printf("Invalid state from client. Ignoring request.\n");
                break;
        }

        db_session_update(db, session.socket, &session);

        host->dealloc_memory(decrypted_packet);
        fflush(stdout);

check_for_other_packets:
        if ((ssize_t) packet_get_size(request_packet) == request_size) {
                return;
        }

        state_machine_handle(
                client,
                request_packet + packet_get_size(request_packet),
                request_size - packet_get_size(request_packet),
                host,
                db);
}
