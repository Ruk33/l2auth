#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#include <sys/stat.h>
#include <time.h>
#endif

#include <stdio.h>
#include "lib.h"

enum lstatus lload(struct lib *lib)
{
#ifdef _WIN32
    enum lstatus result = lloaded;

    WIN32_FILE_ATTRIBUTE_DATA fattr = {0};
    GetFileAttributesEx(lib->path, GetFileExInfoStandard, &fattr);

    if (lib->handle && !CompareFileTime(&lib->loaded_time, &fattr.ftLastWriteTime))
        return luptodate;

    if (lib->handle) {
        result = lreloaded;
        FreeLibrary(lib->handle);
        lib->handle = 0;
    }

    /*
     * create a copy of the dll and then load the copy,
     * not the original file, otherwise, windows will
     * complain when trying to make a change to it.
     */
    snprintf(lib->copy_path, sizeof(lib->copy_path) - 1, ".%s", lib->path);
    if (CopyFileA(lib->path, lib->copy_path, FALSE) == 0)
        return lfailed;

    lib->handle = LoadLibraryA(lib->copy_path);
    if (!lib->handle)
        return lfailed;

    lib->loaded_time = fattr.ftLastWriteTime;

    return result;
#endif

#ifdef __linux__
    enum lstatus result = lloaded;

    struct stat fattr = {0};
    if (stat(lib->path, &fattr) != 0)
        return lfailed;

    if (lib->handle && lib->loaded_time == fattr.st_mtime)
        return luptodate;

    if (lib->handle) {
        result = lreloaded;
        dlclose(lib->handle);
        lib->handle = 0;
    }

    lib->handle = dlopen(lib->path, RTLD_LAZY);
    if (!lib->handle)
        return lfailed;

    lib->loaded_time = fattr.st_mtime;

    return result;
#endif
}

void *lfunction(struct lib *lib, const char *name)
{
#ifdef _WIN32
    return (void *) GetProcAddress(lib->handle, name);
#endif

#ifdef __linux__
    return dlsym(lib->handle, name);
#endif
}

