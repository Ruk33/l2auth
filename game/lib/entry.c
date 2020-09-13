#include <assert.h>
#include <log/log.h>
#include "host.h"
#include "client.h"
#include "entry.h"

struct Entry {
        host_malloc_cb memory_alloc;
        host_mfree_cb memory_free;
        host_send_response_cb send_response;

        struct Client **clients;
        int client_count;
};

void *entry_initialize_server
(host_malloc_cb m, host_mfree_cb f, host_send_response_cb s)
{
        struct Entry *entry = NULL;
        int max_players = 10; // Fix me, get it from server

        entry = m(sizeof(*entry));
        entry->memory_alloc = m;
        entry->memory_free = f;
        entry->send_response = s;

        entry->clients = m(sizeof(*entry->clients) * max_players);
        entry->client_count = 0;

        return entry;
}

void entry_new_conn
(void *entry_p, int client_id)
{
        struct Entry *entry = NULL;
        struct Client *client = NULL;

        entry = entry_p;
        client = client_new(
                client_id,
                entry->memory_alloc,
                entry->memory_free,
                entry->send_response
        );

        entry->clients[client_id] = client;
        entry->client_count++;
}

void entry_handle_request
(void *entry_p, int client_id, unsigned char *buf, size_t buf_size)
{
        assert(buf);
        assert(buf_size > 0);

        struct Entry *entry = NULL;
        struct Client *client = NULL;

        entry = entry_p;
        client = entry->clients[client_id];

        if (!client) {
                log_fatal(
                        "Client with id %d not found, ignoring request",
                        client_id
                );
                return;
        }

        client_handle_request(client, buf, buf_size);
}

// void entry_handle_disconnect(struct Request *request)
// {
//         assert(request);
//         assert(request->client);
//         client_handle_disconnect(request->client);
// }
