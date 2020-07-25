#ifndef L2_SERVER_H
#define L2_SERVER_H

#include <stdlib.h>
#include "response.h"

typedef void  (* queue_response_handler)(struct Response *response);
typedef void* (* alloc_handler)(size_t how_much);
typedef void  (* free_handler)(void *mem);

struct L2Server {
        queue_response_handler queue_response;
        alloc_handler alloc;
        free_handler free;
};

#endif
