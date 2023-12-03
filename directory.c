#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include "directory.h"

struct directory directory_open(char *path)
{
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
}

int directory_next(struct directory *directory)
{
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
}

int directory_create(char *path)
{
#ifdef _WIN32
    if (CreateDirectory(path, 0) || GetLastError() == ERROR_ALREADY_EXISTS)
        return 1;
#endif
    return 0;
}
