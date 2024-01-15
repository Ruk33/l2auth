#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <time.h>
#endif

struct lib {
    void *handle;
    const char *path;

#ifdef _WIN32
    char copy_path[256];
    FILETIME loaded_time;
#endif

#ifdef __linux__
    time_t loaded_time;
#endif
};

enum lstatus {
    lloaded,
    lneedsreload,
    lreloaded,
    luptodate,
    lfailed,
};

/*
 * load dynamic library.
 * the first time, the library will be loaded just normally.
 * 
 * if called again and no changes were made to the library,
 * nothing will be performed and luptodate will be returned.
 * 
 * if there is a new version, the library will be closed and
 * reloaded, returning lreloaded.
 * 
 * if at some point there is an error, lfailed will be returned.
 *
 * important, don't forget to assign the path property
 * before calling this function.
 */
enum lstatus lload(struct lib *lib);
/*
 * get function name from library.
 * on error/not-found, 0 is returned.
 */
void *lfunction(struct lib *lib, const char *name);
