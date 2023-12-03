#ifdef _WIN32
struct directory {
    char *path;
    WIN32_FIND_DATA find_data;
    HANDLE handle;
    
    // for iteration.
    int is_directory;
    char full_path[256];
    char name[64];
};

#define in_directory(path) \
for (struct directory directory = directory_open(path); directory_next(&directory);) \
if (strcmp(directory.name, ".") != 0 && strcmp(directory.name, "..") != 0)

struct directory directory_open(char *path)
{
    struct directory result = {0};
    
    char path_pattern[256] = {0};
    snprintf(path_pattern, sizeof(path_pattern) - 1, "%s/*", path);
    
    result.path = path;
    result.handle = FindFirstFile(path_pattern, &result.find_data);
    
    if (result.handle == INVALID_HANDLE_VALUE)
        result.handle = 0;
    
    return result;
}

int directory_next(struct directory *directory)
{
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
}
#endif
