#include "headers.h"
#include "client_request/handle.h"
#include "storage/server.h"
#include "storage/session.h"
#include "session.h"
#include "game_server.h"

void *game_server_init(host_alloc alloc_memory, host_dealloc dealloc_memory)
{
        assert(alloc_memory);
        assert(dealloc_memory);

        storage_server_t *server_storage = alloc_memory(sizeof(*server_storage));

        printf("Initializing game server.\n");
        storage_server_init(server_storage, alloc_memory, dealloc_memory);

        return server_storage;
}

void game_server_new_connection(int client, storage_server_t *server_storage)
{
        assert(client > 0);
        assert(server_storage);

        session_t session = {0};
        unsigned char key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};

        memcpy(session.encrypt_key, key, sizeof(session.encrypt_key));
        memcpy(session.decrypt_key, key, sizeof(session.decrypt_key));

        printf("Adding new client session.\n");
        storage_session_add(&server_storage->session_storage, client, &session);
}

void game_server_new_request(
        int client,
        byte *request,
        size_t request_size,
        void *data,
        host_alloc alloc_memory,
        host_dealloc dealloc_memory,
        host_send_response_cb send_response,
        host_disconnect_cb disconnect_connection
)
{
        assert(client > 0);
        assert(request);
        assert(request_size);
        assert(data);
        assert(alloc_memory);
        assert(dealloc_memory);
        assert(send_response);
        assert(disconnect_connection);

        storage_server_t *server_storage = data;
        struct Host host = {0};

        host.alloc_memory = alloc_memory;
        host.dealloc_memory = dealloc_memory;
        host.send_response = send_response;
        host.disconnect_connection = disconnect_connection;

        printf("New request from client %d.\n", client);
        printf("Request size: %ld.\n", request_size);

        client_request_handle(client, request, request_size, &host, server_storage);
}

void game_server_client_disconnected(int client, void *data)
{
        assert(client > 0);
        assert(data);

        storage_server_t *server_storage = data;

        printf("Remove client %d from session manager.\n", client);
        storage_session_remove(&server_storage->session_storage, client);
}
