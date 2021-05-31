#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <data_structures/queue.h>
#include "socket.h"
#include "game_server_lib.h"
#include "work_queue.h"

/**
 * TODO: Use memory pool (chunk of memory) instead
 * of allocating/deallocating when memory
 * is required. This way, memory is guaranteed.
 */

static struct Queue *work_queue = NULL;

static pthread_mutex_t thread_mutex     = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  thread_condition = PTHREAD_COND_INITIALIZER;

typedef enum {
        work_new_connection,
        work_disconnected,
        work_client_request,
        work_timer_tick,
        work_log,
        work_send_response,
} work_type_t;

typedef struct {
        work_type_t type;
        int         socket;
        void *      data;
        void *      payload;
        size_t      size;
} work_t;

static void handle_work_when_required(void)
{
        work_t *work  = NULL;
        clock_t start = 0;
        clock_t end   = 0;
        double  delta = 0;

        assert(work_queue);

        while (1) {
                pthread_mutex_lock(&thread_mutex);
                work = queue_dequeue(work_queue);

                if (!work) {
                        pthread_cond_wait(&thread_condition, &thread_mutex);
                        work = queue_dequeue(work_queue);
                }

                pthread_mutex_unlock(&thread_mutex);

                start = clock();

                switch (work->type) {
                case work_new_connection:
                        game_server_lib_new_connection(
                                work->socket, work->data);
                        break;
                case work_disconnected:
                        game_server_lib_handle_disconnect(
                                work->socket, work->data);
                        break;
                case work_client_request:
                        game_server_lib_handle_request(
                                work->socket,
                                work->payload,
                                work->size,
                                work->data);
                        free(work->payload);
                        break;
                case work_timer_tick:
                        game_server_lib_handle_timer_tick(delta, work->data);
                        break;
                case work_log:
                        printf("Log: %s\n", (char *) work->payload);
                        free(work->payload);
                        break;
                case work_send_response:
                        socket_send(work->socket, work->payload, work->size);
                        free(work->payload);
                        break;
                default:
                        break;
                }

                end   = clock();
                delta = (double) ((end - start) / CLOCKS_PER_SEC);

                free(work);
        }
}

static void safe_add_to_queue(work_t *work)
{
        assert(work_queue);
        assert(work);

        pthread_mutex_lock(&thread_mutex);
        queue_enqueue(work_queue, work);
        pthread_cond_signal(&thread_condition);
        pthread_mutex_unlock(&thread_mutex);
}

void work_queue_add_timer_request(void *data)
{
        work_t *work = NULL;

        work       = calloc(1, sizeof(*work));
        work->type = work_timer_tick;
        work->data = data;

        safe_add_to_queue(work);
}

void work_queue_new_connection(void *data, int socket)
{
        work_t *work = NULL;

        work         = calloc(1, sizeof(*work));
        work->type   = work_new_connection;
        work->data   = data;
        work->socket = socket;

        safe_add_to_queue(work);
}

void work_queue_disconnected(void *data, int socket)
{
        work_t *work = NULL;

        work         = calloc(1, sizeof(*work));
        work->type   = work_disconnected;
        work->data   = data;
        work->socket = socket;

        safe_add_to_queue(work);
}

void work_queue_log(char *log)
{
        work_t *work     = NULL;
        size_t  log_size = 0;

        log_size = strlen(log) + 1;

        work          = calloc(1, sizeof(*work));
        work->type    = work_log;
        work->data    = NULL;
        work->socket  = 0;
        work->payload = calloc(1, log_size);
        work->size    = log_size;

        memcpy(work->payload, log, log_size);
        safe_add_to_queue(work);
}

void work_queue_send_response(int socket, void *r, size_t n)
{
        work_t *work = NULL;

        work          = calloc(1, sizeof(*work));
        work->type    = work_send_response;
        work->data    = NULL;
        work->socket  = socket;
        work->payload = calloc(1, n);
        work->size    = n;

        memcpy(work->payload, r, n);
        safe_add_to_queue(work);
}

void work_queue_client_request(
        void *         data,
        int            fd,
        unsigned char *buf,
        size_t         buf_size)
{
        work_t *work = NULL;

        work          = calloc(1, sizeof(*work));
        work->type    = work_client_request;
        work->data    = data;
        work->socket  = fd;
        work->payload = calloc(1, buf_size);
        work->size    = buf_size;

        memcpy(work->payload, buf, buf_size);
        safe_add_to_queue(work);
}

void *work_queue_start(void *p)
{
        work_queue = queue_create(&malloc, &free);
        handle_work_when_required();
        return p;
}
