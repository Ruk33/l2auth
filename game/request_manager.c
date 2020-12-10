#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <data_structures/queue.h>
#include "socket.h"
#include "game_server_lib.h"
#include "response_manager.h"
#include "request_manager.h"

static struct Queue *requests = NULL;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t thread_condition = PTHREAD_COND_INITIALIZER;

struct Request {
        void *data;
        int fd;
        unsigned char buf[65536];
        size_t buf_size;
};

static void handle_requests_when_required(void)
{
        assert(requests);

        struct Request *request = NULL;
        int handle_result = 0;

        while (1)
        {
                pthread_mutex_lock(&thread_mutex);

                request = queue_dequeue(requests);

                if (!request)
                {
                        pthread_cond_wait(&thread_condition, &thread_mutex);
                        request = queue_dequeue(requests);
                }

                pthread_mutex_unlock(&thread_mutex);

                printf("Handling request.\n");
                handle_result = game_server_lib_handle_request(
                        request->fd,
                        request->buf,
                        request->buf_size,
                        request->data,
                        &malloc,
                        &free,
                        &response_manager_add,
                        &socket_close
                );

                if (handle_result == -1)
                {
                        printf("Failed to handle request.\n");
                }

                free(request);
        }
}

void *request_manager_init(void *p)
{
        requests = queue_create(malloc, free);
        handle_requests_when_required();
        return p;
}

void request_manager_add(void *data, int fd, unsigned char *buf, size_t buf_size)
{
        assert(requests);
        assert(data);
        assert(fd > 0);
        assert(buf);
        assert(buf_size);

        struct Request *request = malloc(sizeof(*request));

        request->data = data;
        request->fd = fd;
        request->buf_size = buf_size;

        memcpy(request->buf, buf, buf_size);

        pthread_mutex_lock(&thread_mutex);
        printf("Adding new request.\n");
        queue_enqueue(requests, request);
        pthread_cond_signal(&thread_condition);
        pthread_mutex_unlock(&thread_mutex);
}

