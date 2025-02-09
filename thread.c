#include "thread.h"

void run_in_thread(struct thread *thread, thread_cb *function)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    InitializeCriticalSection(&thread->lock);

    DWORD thread_id = 0;
    CreateThread(0, 0, (void *) function, thread, 0, &thread_id);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_mutex_init(&thread->lock, 0);

    pthread_t thread_id = 0;
    pthread_create(&thread_id, 0, function, thread);
#endif
}

void sleep(int ms)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    Sleep(ms);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    usleep(ms * 1000);
#endif
}

void lock(struct thread *thread)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    EnterCriticalSection(&thread->lock);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_mutex_lock(&thread->lock);
#endif
}

void unlock(struct thread *thread)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    LeaveCriticalSection(&thread->lock);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_mutex_unlock(&thread->lock);
#endif
}
