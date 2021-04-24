#include <headers.h>
#include <db/conn.h>
#include <db/session.h>
#include <state_machine/handle.h>
#include <session.h>
#include "game_server.h"

void *game_server_init(host_alloc alloc_memory, host_dealloc dealloc_memory)
{
        db_conn_t *db = NULL;

        assert(alloc_memory);
        assert(dealloc_memory);

        db = alloc_memory(sizeof(db));

        printf("Initializing game server.\n");
        // db_conn_open(db);

        return db;
}

void game_server_new_connection(int client, db_conn_t *c)
{
        db_conn_t *db      = NULL;
        int        db_open = 0;

        session_t session = { 0 };
        byte_t    key[]   = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };

        assert(client > 0);
        assert(c);

        session.socket = client;
        memcpy(session.encrypt_key, key, sizeof(session.encrypt_key));
        memcpy(session.decrypt_key, key, sizeof(session.decrypt_key));

        db_open = db_conn_open(&db);

        if (!db_open) {
                printf("Failed to open conn to save new session.\n");
                return;
        }

        printf("Adding new client session.\n");
        db_session_add(db, client, &session);
        db_conn_close(db);
        printf("New session saved.\n");
}

void game_server_new_request(
        int                   client,
        byte_t *              request,
        ssize_t               request_size,
        db_conn_t *           db,
        host_alloc            alloc_memory,
        host_dealloc          dealloc_memory,
        host_send_response_cb send_response,
        host_disconnect_cb    disconnect_connection)
{
        assert(request);
        assert(db);
        assert(alloc_memory);
        assert(dealloc_memory);
        assert(send_response);
        assert(disconnect_connection);

        host_t host = { 0 };

        host.alloc_memory          = alloc_memory;
        host.dealloc_memory        = dealloc_memory;
        host.send_response         = send_response;
        host.disconnect_connection = disconnect_connection;

        printf("New request from client %d.\n", client);
        printf("Request size: %ld.\n", request_size);

        state_machine_handle(client, request, request_size, &host, db);
}

void game_server_client_disconnected(int client, db_conn_t *c)
{
        db_conn_t *db      = NULL;
        int        db_open = 0;

        assert(client > 0);
        assert(c);

        db_open = db_conn_open(&db);

        if (!db_open) {
                printf("Failed to open db to delete session %d.\n", client);
                return;
        }

        printf("Remove client %d from session manager.\n", client);
        db_session_remove(db, client);
        db_conn_close(db);
}

void game_server_timer_tick(double delta, db_conn_t *db)
{
        if (delta > 0 && db) {
        }

        // size_t       max              = 0;
        // character_t *characters[10]   = { 0 };
        // size_t       characters_count = 0;

        // if (!db) {
        //         return;
        // }

        // max              = 10 + (size_t) delta;
        // characters_count = storage_character_all(
        //         &server_storage->character_storage, characters, max);

        // for (size_t i = 0; i < characters_count; i++) {
        //         character_move_forward(characters[i]);
        // }
}
