#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>
#endif

#include <stdio.h>
#include "library.h"

enum library_status library_load(struct library *library)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    enum library_status result = library_loaded;

    WIN32_FILE_ATTRIBUTE_DATA fattr = {0};
    GetFileAttributesEx(library->path, GetFileExInfoStandard, &fattr);

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
    snprintf(library->copy_path, sizeof(library->copy_path) - 1, ".%s", library->path);
    if (CopyFileA(library->path, library->copy_path, FALSE) == 0)
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
    if (stat(library->path, &fattr) != 0)
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

    library->handle = dlopen(library->path, RTLD_LAZY);
    if (!library->handle)
        return library_failed;

    library->loaded_time = fattr.st_mtime;
    library->reload_on_next_call = 0;

    return result;
#endif
}

void *library_function(struct library *library, const char *name)
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

