#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <stdio.h>
#include "directory.h"

struct directory directory_open(char *path)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    struct directory result = {0};
    
    char path_pattern[256] = {0};
    snprintf(path_pattern, sizeof(path_pattern) - 1, "%s/*", path);
    
    result.path = path;
    result.handle = FindFirstFile(path_pattern, &result.find_data);
    
    if (result.handle == INVALID_HANDLE_VALUE)
        result.handle = 0;
    
    return result;
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    struct directory result = {0};
    result.path = path;
    result.handle = opendir(path);
    return result;
#endif
}

int directory_next(struct directory *directory)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    if (!directory->handle)
        return 0;
    
    directory->is_directory = directory->find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    snprintf(directory->name, sizeof(directory->name) - 1, "%s", directory->find_data.cFileName);
    snprintf(directory->full_path, 
             sizeof(directory->full_path) - 1, 
             "%s/%s", 
             directory->path, 
             directory->name);
    
    if (!FindNextFile(directory->handle, &directory->find_data)) {
        FindClose(directory->handle);
        directory->handle = 0;
    }
    
    return 1;
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    if (!directory->handle)
        return 0;

    struct dirent *entry = readdir(directory->handle);
    if (!entry) {
        closedir(directory->handle);
        directory->handle = 0;
        return 0;
    }

    directory->is_directory = entry->d_type == DT_DIR;
    /*
     * entry->d_name SHOULD be a null terminated string
     * so there is no need for -1 in dest buffer size.
     */
    snprintf(directory->name, sizeof(directory->name), "%s", entry->d_name);
    snprintf(directory->full_path, 
             sizeof(directory->full_path) - 1, 
             "%s/%s", 
             directory->path, 
             directory->name);

    return 1;
#endif
}

int directory_create(char *path)
{
/*
 * Windows implementation.
 */
#ifdef _WIN32
    if (CreateDirectory(path, 0) || GetLastError() == ERROR_ALREADY_EXISTS)
        return 1;

    return 0;
#endif

/*
 * Linux implementation.
 */
#ifdef __linux__
    /*
     * Create directory for read and write.
     */
    if (mkdir(path, 0666) == 0)
        return 1;

    return 0;
#endif
}
