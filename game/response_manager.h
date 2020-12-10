#ifndef RESPONSE_MANAGER_H
#define RESPONSE_MANAGER_H

#include <stddef.h>

/**
 * Block the current thread where
 * this function gets called to
 * start handling and sending
 * responses added with response_manager_add.
 */
void *response_manager_init(void *);

/**
 * Add new response to the queue.
 * It will be later sent form response_manager_init.
 * Return the amount of bytes to sent.
 */
size_t response_manager_add(int fd, unsigned char *buf, size_t buf_size);

#endif
