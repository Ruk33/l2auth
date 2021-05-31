#include <headers.h>
#include <state_machine/handle.h>
#include <db/conn.h>
#include <db/session.h>
#include <session.h>
#include "host.h"
#include "game_server.h"

/**
 * Messages to be sent from game server back to the host.
 */
typedef enum {
        gs_msg_log,
        gs_msg_response,
} gs_msg_type_t;

/**
 * Messages to be received from the host to the game server.
 */
typedef enum {
        host_msg_new_conn,
        host_msg_client_request,
        host_msg_gs_request,
        host_msg_client_disconnected,
        host_msg_timer_tick,
} host_msg_type_t;

typedef struct {
        int    socket;
        void * response;
        size_t size;
} gs_response_msg_t;

typedef struct {
        int socket;
} host_client_conn_msg_t;

typedef struct {
        int socket;
} host_client_exit_msg_t;

typedef struct {
        int     socket;
        byte_t *request;
        ssize_t size;
} host_request_msg_t;

typedef struct {
        int     socket;
        byte_t *request;
        size_t  size;
} host_gs_request_msg_t;

game_server_t *game_server_init(void *memory, size_t size, msg_to_host_cb cb)
{
        game_server_t *gs = NULL;

        assert(memory);
        assert(size);
        assert(cb);

        gs = memory;

        gs->memory_pool      = (byte_t *) memory + sizeof(*gs);
        gs->memory_pool_size = size - sizeof(*gs);
        gs->msg_to_host      = cb;

        return gs;
}

void *game_server_alloc_mem(game_server_t *gs, size_t n)
{
        byte_t *memory = NULL;
        size_t  used   = 0;

        assert(gs);
        assert(n);

        /**
         * TODO: improve it.
         */
        memcpy(&used, gs->memory_pool, sizeof(used));
        memory = (byte_t *) gs->memory_pool + used;
        n += sizeof(size_t);
        used += n;
        memcpy(gs->memory_pool, &used, sizeof(used));
        memcpy(memory, &n, sizeof(n));

        return memory + sizeof(n);
}

void game_server_free_mem(game_server_t *gs, void *m)
{
        size_t used      = 0;
        size_t used_by_m = 0;

        assert(gs);
        assert(m);

        /**
         * TODO: improve it.
         */
        memcpy(&used, gs->memory_pool, sizeof(used));
        memcpy(&used_by_m, (byte_t *) m - sizeof(used_by_m), sizeof(used_by_m));
        memset((byte_t *) m - sizeof(used_by_m), 0, used_by_m);

        used -= used_by_m;
        memcpy(gs->memory_pool, &used, sizeof(used));
}

void game_server_log(game_server_t *gs, char *msg)
{
        assert(gs);
        assert(msg);
        gs->msg_to_host(gs_msg_log, msg);
}

void game_server_send_response(game_server_t *gs, int socket, void *r, size_t n)
{
        gs_response_msg_t payload = { 0 };

        assert(gs);

        payload.socket   = socket;
        payload.response = r;
        payload.size     = n;

        gs->msg_to_host(gs_msg_response, &payload);
}

static void client_connected(game_server_t *gs, host_client_conn_msg_t *msg)
{
        db_conn_t *db      = NULL;
        int        db_open = 0;

        session_t session = { 0 };
        byte_t    key[]   = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };

        assert(gs);
        assert(msg);

        session.socket = msg->socket;
        memcpy(session.encrypt_key, key, sizeof(session.encrypt_key));
        memcpy(session.decrypt_key, key, sizeof(session.decrypt_key));

        db_open = db_conn_open(&db);

        if (!db_open) {
                game_server_log(gs, "Failed to open db to save new session.");
                /**
                 * TODO: disconnect client?
                 */
                return;
        }

        game_server_log(gs, "Adding new client session.");
        db_session_add(db, session.socket, &session);
        db_conn_close(db);
        game_server_log(gs, "New session saved.");
}

static void client_request(game_server_t *gs, host_request_msg_t *msg)
{
        host_t host = { 0 };

        assert(gs);
        assert(msg);

        host.gs        = gs;
        host.alloc_mem = (void *(*) (void *, size_t)) & game_server_alloc_mem;
        host.free_mem  = (void (*)(void *, void *)) & game_server_free_mem;
        host.log       = (void (*)(void *, char *)) & game_server_log;
        host.send_response = (void (*)(void *, int, void *, size_t)) &
                             game_server_send_response;

        state_machine_handle(&host, msg->socket, msg->request, msg->size);
}

static void gs_request(game_server_t *gs, host_gs_request_msg_t *msg)
{
        /**
         * TODO: Implement.
         * NOTE: Sometimes the game server
         * needs to send multiple responses
         * from a request. Instead of sending
         * those immediatly, it MAY be better
         * to queue those up so the work is
         * distributed evenly. I COULD BE WRONG
         * about this and it could be better to
         * just send the entire thing at once.
         */
        assert(gs);
        assert(msg);
}

static void client_disconnected(game_server_t *gs, host_client_exit_msg_t *msg)
{
        db_conn_t *db = NULL;

        int  db_open = 0;
        char log[64] = { 0 };

        assert(gs);
        assert(msg);

        db_open = db_conn_open(&db);

        if (!db_open) {
                snprintf(
                        log,
                        sizeof(log),
                        "Couldn't open db to delete client %d.",
                        msg->socket);
                game_server_log(gs, log);
                return;
        }

        db_session_remove(db, msg->socket);
        db_conn_close(db);

        snprintf(log, sizeof(log), "Remove client %d.", msg->socket);
        game_server_log(gs, log);
}

void game_server_request(game_server_t *gs, int t, void *payload, ssize_t n)
{
        host_msg_type_t type = 0;

        assert(gs);
        assert(payload);

        /**
         * NOTE: n may not be needed at all
         */
        assert(n);

        type = (host_msg_type_t) t;

        /**
         * NOTE: Not sure about sending entire structs inside of payload...
         */
        switch (type) {
        case host_msg_new_conn:
                client_connected(gs, (host_client_conn_msg_t *) payload);
                break;
        case host_msg_client_request:
                client_request(gs, (host_request_msg_t *) payload);
                break;
        case host_msg_gs_request:
                gs_request(gs, (host_gs_request_msg_t *) payload);
                break;
        case host_msg_client_disconnected:
                client_disconnected(gs, (host_client_exit_msg_t *) payload);
                break;
        case host_msg_timer_tick:
                /**
                 * TODO: implement
                 */
                break;
        default:
                game_server_log(gs, "Invalid message from host...");
                break;
        }
}
