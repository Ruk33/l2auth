#ifndef HOST_H
#define HOST_H

#include <stdlib.h>

typedef void *(* host_malloc_cb)
(size_t how_much);

typedef void (* host_mfree_cb)
(void *memory);

typedef void (* host_send_response_cb)
(int client_id, unsigned char *buf, size_t buf_size);

#endif
