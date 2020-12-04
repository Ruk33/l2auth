#include <stdlib.h>
#include "server.h"

int main(/* int argc, char **argv */)
{
        unsigned short port = 2106;
        size_t max_connections = 10;
        return server_start(port, max_connections);
}
