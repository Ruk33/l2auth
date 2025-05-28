struct lock {
#ifdef _WIN32
    CRITICAL_SECTION handle;
#endif

#ifdef __linux__
    pthread_mutex_t handle;
#endif
};

typedef int thread_cb(void *data);

void thread_run(void *data, thread_cb *function)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    DWORD thread_id = 0;
    CreateThread(0, 0, (void *) function, data, 0, &thread_id);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_t thread_id = 0;
    pthread_create(&thread_id, 0, function, data);
#endif
}

void thread_sleep(int ms)
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

struct lock lock_init(void)
{
/*
 * Windows implementation
 */
#ifdef _WIN32
    struct lock result = {0};

    InitializeCriticalSection(&result.handle);

    return result;
#endif

/*
 * Linux implementation
 */
#ifdef __linux__
    struct lock result = {0};

    pthread_mutex_init(&result.handle, 0);

    return result;
#endif
}

void lock(struct lock lock)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    EnterCriticalSection(&lock.handle);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_mutex_lock(&lock.handle);
#endif
}

void unlock(struct lock lock)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    LeaveCriticalSection(&lock.handle);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    pthread_mutex_unlock(&lock.handle);
#endif
}
