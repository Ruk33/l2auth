#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#include <sys/stat.h>
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

enum library_status load_library(struct library *library, const char *path)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    enum library_status result = library_loaded;

    WIN32_FILE_ATTRIBUTE_DATA fattr = {0};
    GetFileAttributesEx(path, GetFileExInfoStandard, &fattr);

    /*
     * Library is up to date?
     */
    if (library->handle && !CompareFileTime(&library->loaded_time, &fattr.ftLastWriteTime))
        return library_up_to_date;

    /*
     * Library is loaded and needs to be reloaded.
     */
    if (library->handle) {
        if (!library->reload_on_next_call) {
            library->reload_on_next_call = 1;
            return library_needs_reload;
        }

        result = library_reloaded;
        FreeLibrary(library->handle);
        library->handle = 0;
    }

    /*
     * Create a copy of the dll and then load the copy,
     * not the original file, otherwise, windows will
     * complain when trying to make a change to it.
     */
    snprintf(library->copy_path, sizeof(library->copy_path) - 1, ".%s", path);
    if (CopyFileA(path, library->copy_path, FALSE) == 0)
        return library_failed;

    library->handle = LoadLibraryA(library->copy_path);
    if (!library->handle)
        return library_failed;

    library->loaded_time = fattr.ftLastWriteTime;
    library->reload_on_next_call = 0;

    return result;
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    enum library_status result = library_loaded;

    struct stat fattr = {0};
    if (stat(path, &fattr) != 0)
        return library_failed;

    /*
     * Library up to date?
     */
    if (library->handle && library->loaded_time == fattr.st_mtime)
        return library_up_to_date;

    /*
     * Library is loaded and needs to be reloaded.
     */
    if (library->handle) {
        if (!library->reload_on_next_call) {
            library->reload_on_next_call = 1;
            return library_needs_reload;
        }

        result = library_reloaded;
        dlclose(library->handle);
        library->handle = 0;
    }

    library->handle = dlopen(path, RTLD_LAZY);
    if (!library->handle)
        return library_failed;

    library->loaded_time = fattr.st_mtime;
    library->reload_on_next_call = 0;

    return result;
#endif
}

void *load_function(struct library *library, const char *name)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    return (void *) GetProcAddress(library->handle, name);
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    return dlsym(library->handle, name);
#endif
}
