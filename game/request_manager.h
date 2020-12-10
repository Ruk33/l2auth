#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <stddef.h>

/**
 * Block the current thread where
 * this function gets called to
 * start handling requests added
 * with request_manager_add.
 */
void *request_manager_init(void *);

/**
 * Add new request to the queue.
 * It will be later be handled
 * with request_manager_init.
 */
void request_manager_add(void *data, int fd, unsigned char *buf, size_t buf_size);

#endif
