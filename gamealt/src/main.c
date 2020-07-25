#include "mem.h"
#include "server.h"

int main()
{
        mem_init();
        server_start(7777, 10);
        return 0;
}
