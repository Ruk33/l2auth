#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdlib.h>
#include "client_id.h"

struct Response {
        client_id client_id;
        char *buf;
        size_t buf_size;
};

#endif
