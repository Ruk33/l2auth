#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <log/log.h>
#include "code.h"
#include "connection_manager.h"
#include "queue.h"
#include "response_manager.h"

struct Response {
        int client_id;
        unsigned char *buf;
        size_t buf_size;
};

struct ResponseQueue {
        struct Queue *queue;
};

static struct ResponseQueue *responses = NULL;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t thread_condition = PTHREAD_COND_INITIALIZER;

void response_manager_init
(void)
{
        responses = code_malloc(sizeof(*responses));
        responses->queue = queue_new();
}

void response_manager_enqueue
(int client_id, unsigned char *buf, size_t buf_size)
{
        assert(responses);
        assert(responses->queue);
        assert(buf);
        assert(buf_size > 0);

        struct Response *response = NULL;

        response = code_malloc(sizeof(*response));
        response->client_id = client_id;
        response->buf = code_malloc(buf_size);
        response->buf_size = buf_size;

        memcpy(response->buf, buf, buf_size);

        pthread_mutex_lock(&thread_mutex);
        queue_enqueue(responses->queue, response);
        pthread_cond_signal(&thread_condition);
        pthread_mutex_unlock(&thread_mutex);
}

static struct Response *response_manager_dequeue
(void)
{
        assert(responses);
        assert(responses->queue);
        return queue_dequeue(responses->queue);
}

void *response_manager_start_handling_responses
(void *p)
{
        assert(responses);

        struct Response *response = NULL;

        log_info("Starting to handle responses");

        while (1) {
                pthread_mutex_lock(&thread_mutex);

                response = response_manager_dequeue();

                if (!response) {
                        pthread_cond_wait(&thread_condition, &thread_mutex);
                        response = response_manager_dequeue();
                }

                pthread_mutex_unlock(&thread_mutex);

                connection_manager_send_response(
                        response->client_id,
                        response->buf,
                        response->buf_size
                );

                code_mfree(response->buf);
                code_mfree(response);
        }

        return NULL;
}
