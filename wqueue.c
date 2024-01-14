#include <windows.h>
#include "wqueue.h"

static unsigned long wqueue_thread(void *p)
{
    struct wqueue *q = (struct wqueue *) p;
    while (1) {
        int close = 0;
        EnterCriticalSection(&q->lock);
        // close = q->closed && !q->wcount;
        close = q->closed;
        LeaveCriticalSection(&q->lock);
        // add a signal for this, so wclose can block
        // until all work is done and then released
        // when everything is done and ready to properly close.
        if (close)
            break;

        WaitForSingleObject(q->wpending, INFINITE);
        EnterCriticalSection(&q->lock);
        void *work = q->work[0];
        for (size_t i = 1; i < q->wcount; i++)
            q->work[i - 1] = q->work[i];
        q->wcount--;
        LeaveCriticalSection(&q->lock);
        q->worker((void *) q, work);
    }

    CloseHandle(q->wpending);
    CloseHandle(q->thread);
    DeleteCriticalSection(&q->lock);

    return 0;
}

void wstart(struct wqueue *q, worker *worker)
{
    InitializeCriticalSection(&q->lock);
    q->closed = 0;
    q->worker = worker;
    q->wpending = CreateEvent(0, FALSE, FALSE, 0);
    q->thread = CreateThread(0, 0, wqueue_thread, q, 0, 0);
}

void wpush(struct wqueue *q, void *work)
{
    if (q->closed)
        return;
    EnterCriticalSection(&q->lock);
    q->work[q->wcount++] = work;
    LeaveCriticalSection(&q->lock);
    SetEvent(q->wpending);
}

void wclose(struct wqueue *q)
{
    if (q->closed)
        return;
    EnterCriticalSection(&q->lock);
    q->closed = 1;
    LeaveCriticalSection(&q->lock);
}

#ifdef run_wqueue
// example on how to use these functions
#include <stdio.h>

void do_work(struct wqueue *q, void *w)
{
    static int tmp = 42;
    int value = *(int *) w;
    if (value == 2) {
        fprintf(stderr, "adding new value because we found 2\n");
        wpush(q, &tmp);
    }
    fprintf(stderr, "new work! value is %d\n", value);
}

int main()
{
    struct wqueue q = {0};
    wstart(&q, do_work);

    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        wpush(&q, values + i);
        // wclose(&q);
        Sleep(1000);
    }
    fprintf(stderr, "all done!\n");
}
#endif
