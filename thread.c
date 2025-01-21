#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <pthread.h>
#endif

#include "thread.h"

void run_in_thread(thread_cb *function, void *buf)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    DWORD thread_id = 0;
    CreateThread(0, 0, (void *) function, buf, 0, &thread_id);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_t thread_id = 0;
    pthread_create(&thread_id, 0, function, buf);
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
