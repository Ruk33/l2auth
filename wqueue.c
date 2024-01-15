#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <pthread.h>
#endif

#include "wqueue.h"

#ifdef _WIN32
static DWORD wqueue_thread(LPVOID p)
#endif
#ifdef __linux__
static void *wqueue_thread(void *p)
#endif
{
    struct wqueue *q = (struct wqueue *) p;

#ifdef _WIN32
    while (1) {
        int close = 0;

        WaitForSingleObject(q->lock, INFINITE);
        close = q->closed;
        ReleaseMutex(q->lock);
        if (close)
            break;

        WaitForSingleObject(q->wpending, INFINITE);
        WaitForSingleObject(q->lock, INFINITE);
        void *work = q->work[0];
        for (size_t i = 1; i < q->wcount; i++)
            q->work[i - 1] = q->work[i];
        q->wcount--;
        ReleaseMutex(q->lock);
        
        q->worker((void *) q, work);
    }

    CloseHandle(q->wpending);
    CloseHandle(q->thread);
    CloseHandle(q->lock);
#endif

#ifdef __linux__
    while (1) {
        int close = 0;

        pthread_mutex_lock(&q->lock);
        close = q->closed;
        pthread_mutex_unlock(&q->lock);
        if (close)
            break;

        pthread_cond_wait(&q->wpending, &q->lock);
        pthread_mutex_lock(&q->lock);
        void *work = q->work[0];
        for (size_t i = 1; i < q->wcount; i++)
            q->work[i - 1] = q->work[i];
        q->wcount--;
        pthread_mutex_unlock(&q->lock);
        
        q->worker((void *) q, work);
    }

    pthread_cond_destroy(&q->wpending);
    pthread_mutex_destroy(&q->lock);
    pthread_exit(0);
#endif

    return 0;
}

void wstart(struct wqueue *q, worker *worker)
{
    q->closed = 0;
    q->worker = worker;

#ifdef _WIN32
    q->lock = CreateMutex(0, FALSE, 0);
    q->wpending = CreateEvent(0, FALSE, FALSE, 0);
    q->thread = CreateThread(0, 0, wqueue_thread, q, 0, 0);
#endif

#ifdef __linux__
    pthread_mutex_init(&q->lock, 0);
    pthread_cond_init(&q->wpending, 0);
    pthread_create(&q->thread, 0, wqueue_thread, q);
#endif
}

void wpush(struct wqueue *q, void *work)
{
    if (q->closed)
        return;

#ifdef _WIN32
    WaitForSingleObject(q->lock, INFINITE);
    q->work[q->wcount++] = work;
    ReleaseMutex(q->lock);
    SetEvent(q->wpending);
#endif

#ifdef __linux__
    pthread_mutex_lock(&q->lock);
    q->work[q->wcount++] = work;
    pthread_cond_signal(&q->wpending);
    pthread_mutex_unlock(&q->lock);
#endif
}

void wclose(struct wqueue *q)
{
    if (q->closed)
        return;

#ifdef _WIN32
    WaitForSingleObject(q->lock, INFINITE);
    q->closed = 1;
    ReleaseMutex(q->lock);
#endif

#ifdef __linux__
    pthread_mutex_lock(&q->lock);
    q->closed = 1;
    pthread_mutex_unlock(&q->lock);
#endif
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
