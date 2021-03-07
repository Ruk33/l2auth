#include <assert.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <data_structures/queue.h>
#include "memory.h"
#include "socket.h"
#include "game_server_lib.h"
#include "work_queue.h"

static struct Queue *work_queue = NULL;

static pthread_mutex_t thread_mutex     = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  thread_condition = PTHREAD_COND_INITIALIZER;

typedef enum {
        work_timer_tick,
        work_client_request,
} work_type_t;

typedef struct {
        work_type_t    type;
        int            fd;
        void *         data;
        unsigned char *buf;
        size_t         buf_size;
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

                delta = (double) ((end - start) / CLOCKS_PER_SEC);
                start = clock();

                switch (work->type) {
                case work_timer_tick:
                        game_server_lib_handle_timer_tick(delta, work->data);
                        break;
                case work_client_request:
                        game_server_lib_handle_request(
                                work->fd,
                                work->buf,
                                work->buf_size,
                                work->data,
                                &memory_alloc,
                                &memory_free,
                                &socket_send,
                                &socket_close);
                        break;
                default:
                        break;
                }

                end = clock();

                memory_free(work);
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

        work       = memory_alloc(sizeof(*work));
        work->type = work_timer_tick;
        work->data = data;

        safe_add_to_queue(work);
}

void work_queue_add_client_request(
        void *         data,
        int            fd,
        unsigned char *buf,
        size_t         buf_size)
{
        work_t *work = NULL;

        work           = memory_alloc(sizeof(*work));
        work->type     = work_client_request;
        work->data     = data;
        work->fd       = fd;
        work->buf      = memory_alloc(buf_size);
        work->buf_size = buf_size;

        memcpy(work->buf, buf, buf_size);
        safe_add_to_queue(work);
}

void *work_queue_start(void *p)
{
        work_queue = queue_create(&memory_alloc, &memory_free);
        handle_work_when_required();
        return p;
}
