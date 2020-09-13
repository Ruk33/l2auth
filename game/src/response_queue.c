#include <assert.h>
#include <string.h>
#include <log/log.h>
#include "code.h"
#include "connection_manager.h"
#include "queue.h"
#include "response_queue.h"

struct Response {
        int client_id;
        unsigned char *buf;
        size_t buf_size;
};

struct ResponseQueue {
        struct Queue *queue;
};

static struct ResponseQueue *responses = NULL;

void response_queue_init
(void)
{
        responses = code_malloc(sizeof(*responses));
        responses->queue = queue_new();
}

void response_queue_enqueue
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
        queue_enqueue(responses->queue, response);
}

static struct Response *response_queue_dequeue
(void)
{
        assert(responses);
        assert(responses->queue);
        return queue_dequeue(responses->queue);
}

void *response_queue_start_handling_responses
(void *p)
{
        assert(responses);

        struct Response *response = NULL;

        log_info("Starting to handle responses");

        while (1) {
                response = response_queue_dequeue();

                if (!response) continue;

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
