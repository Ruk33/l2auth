#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <data_structures/queue.h>
#include "socket.h"
#include "response_manager.h"

static struct Queue *responses = NULL;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t thread_condition = PTHREAD_COND_INITIALIZER;

struct Response {
        int fd;
        unsigned char buf[65536];
        size_t buf_size;
};

static void send_requests_when_required(void)
{
        assert(responses);

        struct Response *response = NULL;

        while (1)
        {
                pthread_mutex_lock(&thread_mutex);

                response = queue_dequeue(responses);

                if (!response)
                {
                        pthread_cond_wait(&thread_condition, &thread_mutex);
                        response = queue_dequeue(responses);
                }

                pthread_mutex_unlock(&thread_mutex);

                printf("Sending response.\n");
                socket_send(
                        response->fd,
                        response->buf,
                        response->buf_size
                );

                free(response);
        }
}

void *response_manager_init(void *p)
{
        responses = queue_create(malloc, free);
        send_requests_when_required();
        return p;
}

size_t response_manager_add(int fd, unsigned char *buf, size_t buf_size)
{
        assert(responses);
        assert(fd > 0);
        assert(buf);
        assert(buf_size);

        struct Response *response = malloc(sizeof(*response));

        response->fd = fd;
        response->buf_size = buf_size;

        memcpy(response->buf, buf, buf_size);

        pthread_mutex_lock(&thread_mutex);
        printf("Adding new response.\n");
        queue_enqueue(responses, response);
        pthread_cond_signal(&thread_condition);
        pthread_mutex_unlock(&thread_mutex);

        return buf_size;
}

