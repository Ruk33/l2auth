#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include "code.h"
#include "queue.h"
#include "request_queue.h"

struct Request {
        void *server_data;
        int client_id;
        unsigned char *buf;
        size_t buf_size;
};

struct RequestQueue {
        struct Queue *queue;
};

static struct RequestQueue *requests = NULL;

void request_queue_init
(void)
{
        requests = code_malloc(sizeof(*requests));
        requests->queue = queue_new();
}

void request_queue_enqueue
(void *server_data, int client_id, unsigned char *buf, size_t buf_size)
{
        assert(requests);
        assert(requests->queue);
        assert(buf);
        assert(buf_size > 0);

        struct Request *request = NULL;

        request = code_malloc(sizeof(*request));
        request->server_data = server_data;
        request->client_id = client_id;
        request->buf = code_malloc(buf_size);
        request->buf_size = buf_size;

        memcpy(request->buf, buf, buf_size);
        queue_enqueue(requests->queue, request);
}

static struct Request *request_queue_dequeue
(void)
{
        assert(requests);
        assert(requests->queue);
        return queue_dequeue(requests->queue);
}

void *request_queue_start_handling_requests
(void *p)
{
        assert(requests);

        struct Request *request = NULL;

        log_info("Starting to handle requests");

        while (1) {
                request = request_queue_dequeue();

                if (!request) continue;

                code_request_handle(
                        request->server_data,
                        request->client_id,
                        request->buf,
                        request->buf_size
                );

                code_mfree(request->buf);
                code_mfree(request);
        }

        return NULL;
}
