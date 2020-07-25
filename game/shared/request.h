#ifndef REQUEST_H
#define REQUEST_H

#include <stdlib.h>

struct Request {
        void *client;
        char *buf;
        size_t buf_size;
};

#endif
