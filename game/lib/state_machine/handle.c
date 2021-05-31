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

static void log_session_not_found(host_t *h, int socket)
{
        char err_fmt[] = "Warning: session for client %d not found.";
        char err[64]   = { 0 };

        assert(h);

        snprintf(err, sizeof(err), err_fmt, socket);
        h->log(h->gs, err);
        h->log(h->gs, "Request will be ignored.");
}

void state_machine_handle(host_t *h, int socket, byte_t *raw_req, ssize_t size)
{
        db_conn_t *db = NULL;

        int db_open       = 0;
        int session_found = 0;

        request_t request = { 0 };
        session_t session = { 0 };

        packet *decrypted_packet = NULL;

        packet_size p_size = 0;

        assert(h);
        assert(raw_req);

        /**
         * NOTE: Since only one request will be handled at
         * a time, shouldn't just be used 1 db conn?
         * This way we can also guarantee the databse will be
         * properly opened for queries.
         */
        db_open = db_conn_open(&db);

        if (!db_open) {
                h->log(h->gs, "Couldn't open db to handle request.");
                goto check_for_other_packets;
        }

        session_found = db_session_get(db, &session, socket);

        if (!session_found) {
                log_session_not_found(h, socket);
                goto check_for_other_packets;
        }

        /**
         * TODO: Refactor, don't use 65536
         */
        decrypted_packet = h->alloc_mem(h->gs, 65536);
        p_size           = packet_get_size(raw_req);

        util_session_decrypt_packet(
                db, socket, decrypted_packet, raw_req, p_size);
        util_session_encrypt_connection(db, socket);

        request.host    = h;
        request.socket  = socket;
        request.storage = db;
        request.packet  = decrypted_packet;
        request.size    = size;

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
                h->log(h->gs, "Invalid request. Will be ignored...");
                break;
        }

        db_conn_close(db);

        h->free_mem(h->gs, decrypted_packet);

check_for_other_packets:
        if ((ssize_t) p_size == size) {
                return;
        }

        state_machine_handle(h, socket, raw_req + p_size, size - p_size);
}
