#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <time.h>
#endif

struct library {
    void *handle;
    int reload_on_next_call;

#ifdef _WIN32
    char copy_path[256];
    FILETIME loaded_time;
#endif

#ifdef __linux__
    time_t loaded_time;
#endif
};

enum library_status {
    library_loaded,
    library_needs_reload,
    library_reloaded,
    library_up_to_date,
    library_failed,
};

/*
 * Load dynamic library.
 * the first time, the library will be loaded just normally.
 * 
 * If called again and no changes were made to the library,
 * nothing will be performed and library_up_to_date will be returned.
 * 
 * If there is a new version, the library will be closed and
 * reloaded, returning library_reloaded.
 * 
 * If at some point there is an error, library_failed will be returned.
 */
enum library_status load_library(struct library *library, const char *path);
/*
 * Get function name from library.
 * On error/not-found, 0 is returned.
 */
void *load_function(struct library *libray, const char *name);
