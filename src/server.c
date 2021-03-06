#include <assert.h>
#include "include/config.h"
#include "include/util.h"
#include "include/log.h"
#include "include/server.h"

static server_t servers[MAX_SERVERS] = { 0 };
static size_t server_count           = 0;

void server_add(server_t *server)
{
        assert(server);

        if (server_count == MAX_SERVERS) {
                log("ERROR: Update MAX_SERVERS in config.");
                return;
        }

        servers[server_count] = *server;
        server_count += 1;
}

server_t *server_all(void)
{
        return servers;
}

size_t server_all_count(void)
{
        return server_count;
}
