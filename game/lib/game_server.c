#include <headers.h>
#include <state_machine/handle.h>
#include <storage/server.h>
#include <storage/session.h>
#include <session.h>
#include "game_server.h"

void *game_server_init(host_alloc alloc_memory, host_dealloc dealloc_memory)
{
        assert(alloc_memory);
        assert(dealloc_memory);

        storage_server_t *server_storage =
                alloc_memory(sizeof(*server_storage));

        printf("Initializing game server.\n");
        storage_server_init(server_storage, alloc_memory, dealloc_memory);

        return server_storage;
}

void game_server_new_connection(int client, storage_server_t *server_storage)
{
        assert(client > 0);
        assert(server_storage);

        session_t     session = { 0 };
        unsigned char key[] = { 0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87 };

        session.socket = client;
        memcpy(session.encrypt_key, key, sizeof(session.encrypt_key));
        memcpy(session.decrypt_key, key, sizeof(session.decrypt_key));

        printf("Adding new client session.\n");
        storage_session_add(&server_storage->session_storage, client, &session);
}

void game_server_new_request(
        int                   client,
        byte_t *              request,
        ssize_t               request_size,
        storage_server_t *    server_storage,
        host_alloc            alloc_memory,
        host_dealloc          dealloc_memory,
        host_send_response_cb send_response,
        host_disconnect_cb    disconnect_connection)
{
        assert(request);
        assert(server_storage);
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

        state_machine_handle(
                client, request, request_size, &host, server_storage);
}

void game_server_client_disconnected(
        int               client,
        storage_server_t *server_storage)
{
        assert(client > 0);
        assert(server_storage);

        printf("Remove client %d from session manager.\n", client);
        storage_session_remove(&server_storage->session_storage, client);
}

void game_server_timer_tick(double delta, storage_server_t *server_storage)
{
        if (server_storage) {
        }
        printf("Timer, delta: %f\n", delta);
}
